#include <sbi/sbi_timer.h>
#include "fuzzy-time.h"
#include "enclave.h"
#include "cpu.h"
#include <sbi/sbi_console.h> // for sbi_printf() for debugging
#include "string.h"

#include "sbi/sbi_ipi.h"
#include "sbi/sbi_hsm.h"
#include "sbi/sbi_domain.h"

#define GRANULARITY_MS 10

// TODO(chungmcl): Remove! For debugging
#define FUZZ_ON 0

unsigned long fuzz_clock_ticks;
unsigned long t_end_ticks = 0;
unsigned long granularity_ticks;
unsigned long ticks_per_ms;

int clock_ipi_event_id = -1;
static struct sbi_ipi_event_ops clock_ipi_event_ops;
// bitmask of enclaves registered for fuzzy clock IPIs
short clock_ipi_registered_enclaves;

void handle_clock_ipi(struct sbi_scratch *scratch) {
  unsigned int hart_id = current_hartid();
  enclave_id eid = cpu_get_enclave_id() + 1;
  // sbi_printf("!!! handle_clock_ipi on core %d\n", hart_id);

  if (cpu_is_enclave_context()) {
    sbi_printf("!!! cpu_is_enclave_context == true && hart_id: %d\n", hart_id);
    short is_enclave_registered = (clock_ipi_registered_enclaves >> eid) & 0b1;
    if (is_enclave_registered) {
      sbi_printf("!!! is enclave %d on hart %d registered?: %d\n", eid, hart_id, (int)is_enclave_registered);
      // csr_set(CSR_MIP, MIP_SSIP);
    }
  }
}

void reg_clock_ipi(void (* process)(struct sbi_scratch *scratch)) {
  enclave_id eid = cpu_get_enclave_id() + 1;

  clock_ipi_registered_enclaves = clock_ipi_registered_enclaves | (0b1 << eid);

  sbi_printf("reg_clock_ipi called:\n\tmhartid: %d\n\teid: %d\n\tprocess: %p\n", current_hartid(), eid, process);
}

void send_clock_ipi() {
  ulong mask = 0;
  sbi_hsm_hart_interruptible_mask(sbi_domain_thishart_ptr(), 0, &mask);
  sbi_ipi_send_many(mask, 0, clock_ipi_event_id, NULL);
  // sbi_printf("send_clock_ipi with mask: %ld and event_id: %d\n", mask, clock_ipi_event_id);
}

void fuzzy_time_init() {
  // (ticks / second) * (seconds / milliseconds) = (ticks / milliseconds)
  ticks_per_ms = sbi_timer_get_device()->timer_freq / 1000;
  // milliseconds * (ticks / milliseconds) = ticks
  granularity_ticks = GRANULARITY_MS * ticks_per_ms;
  unsigned long t = sbi_timer_value();
  t_end_ticks = t - (t % granularity_ticks);

  if (clock_ipi_event_id == -1) {
    clock_ipi_registered_enclaves = 0;
    clock_ipi_event_ops.name[0] = 'x';
    clock_ipi_event_ops.name[1] = '\0';
    clock_ipi_event_ops.process = handle_clock_ipi;
    clock_ipi_event_id = sbi_ipi_event_create(&clock_ipi_event_ops);
    sbi_printf("Fuzzy Clock IPI event created with ID: %d\n", clock_ipi_event_id);
  }

  sbi_printf("Fuzzy Clock initialized with granularity_ticks == %lu\n", granularity_ticks);
}

unsigned long prev_time; // TODO(chungmcl): remove! for debugging
unsigned long update_fuzzy_clock() {
#if FUZZ_ON
  // Fuzz On
  uint64_t now_ticks = sbi_timer_value();
  
  uint64_t floored_now_ticks = now_ticks - (now_ticks % granularity_ticks);
  if (fuzz_clock_ticks < floored_now_ticks) {
    fuzz_clock_ticks = floored_now_ticks;
    // sbi_printf("!!! fuzz_clock_ticks updated to %lu @ %lu, dif: %lu\n", fuzz_clock_ticks, now_ticks, now_ticks - prev_time);
    prev_time = now_ticks;
  }
  
  // TODO(chungmcl): verify that sbi_sm_random is indeed uniformly random
  return sbi_sm_random() % (granularity_ticks + 1);
#else
  // Fuzz Off
  uint64_t now_ticks = sbi_timer_value();
  fuzz_clock_ticks = now_ticks - (now_ticks % granularity_ticks);

  send_clock_ipi();

  return granularity_ticks;
#endif
}

void fix_time_interval(unsigned long t) {
  // back compute
  //
  // t_end_ticks == when the current tick ends
  // fuzz_clock_ticks == current fuzzed time
  // g == granularity/interval max
  //
  // keep going until t_end is in the future

  // may need to have safety mechanism to check for verrrry long gaps
  // between real_time and t_end_ticks? and skip this process
  while (t_end_ticks < t) {
    fuzz_clock_ticks = t_end_ticks - (t_end_ticks % granularity_ticks);
    // TODO: ENSURE UNIFORMLY RANDOM! IT ISN'T RIGHT NOW!
    unsigned long rand = sbi_sm_random();
    // sbi_printf("sbi_sm_random: %lu\n", rand);
    unsigned long to_add = rand % (granularity_ticks + 1);
    t_end_ticks += to_add;
    // sbi_printf("t_end_ticks += %lu\n", to_add);
  }
}

unsigned long wait_until_epoch() {
  unsigned long start_fuzz_ticks = fuzz_clock_ticks;
  while (start_fuzz_ticks == fuzz_clock_ticks) {
    // sbi_printf("start_fuzz_ticks: %lu\n", start_fuzz_ticks);
    // sbi_printf("fuzz_clock_ticks: %lu\n", fuzz_clock_ticks);
    unsigned long t = sbi_timer_value();
    unsigned long delta_ticks = t_end_ticks - t;
    // ticks / (ticks / milliseconds) = milliseconds
    unsigned long delay_ms = delta_ticks / ticks_per_ms;
    if (t < t_end_ticks) {
      // sbi_printf("ms delay: %lu\n", delay_ms);
      sbi_timer_mdelay(delay_ms);
    }
    fix_time_interval(t);
  }
  return fuzz_clock_ticks;
}

void wait_for_ms(unsigned long ms) {
  sbi_timer_mdelay(ms);
}

unsigned long get_time_ticks() {
  /*enclave_id eid = cpu_get_enclave_id();
  struct enclave* enclave = get_enclave(eid);
  if (enclave->fuzzy_status == FUZZ_ENABLED)*/ {
    // fix_time_interval(sbi_timer_value());
    return fuzz_clock_ticks;
  }
}

unsigned long get_granularity_ticks() {
  return granularity_ticks;
}