#include <sbi/sbi_timer.h>
#include "fuzzy-clock.h"
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

volatile unsigned long fuzz_clock_ticks;
unsigned long granularity_ticks;
unsigned long ticks_per_ms;

int clock_ipi_event_id = -1;
static struct sbi_ipi_event_ops clock_ipi_event_ops;
short clock_ipi_registered_enclaves; // bitmask of enclaves registered for fuzzy clock IPIs

// Private functions
void handle_clock_ipi(struct sbi_scratch *scratch);
void send_clock_ipi();

void fuzzy_clock_init() {
  if (clock_ipi_event_id == -1) {
    // (ticks / second) * (seconds / milliseconds) = (ticks / milliseconds)
    ticks_per_ms = sbi_timer_get_device()->timer_freq / 1000;
    // milliseconds * (ticks / milliseconds) = ticks
    granularity_ticks = GRANULARITY_MS * ticks_per_ms;
    clock_ipi_registered_enclaves = 0;
    clock_ipi_event_ops.name[0] = 'x';
    clock_ipi_event_ops.name[1] = '\0';
    clock_ipi_event_ops.process = handle_clock_ipi;
    clock_ipi_event_id = sbi_ipi_event_create(&clock_ipi_event_ops);
    sbi_printf("Fuzzy Clock IPI event created with ID: %d\n", clock_ipi_event_id);
    sbi_printf("Fuzzy Clock initialized with granularity_ticks == %lu\n", granularity_ticks);
  }
}

// unsigned long prev_time; // TODO(chungmcl): remove! for debugging
unsigned long update_fuzzy_clock() {
#if FUZZ_ON
  // Fuzz On
  uint64_t now_ticks = sbi_timer_value();
  
  uint64_t floored_now_ticks = now_ticks - (now_ticks % granularity_ticks);
  if (fuzz_clock_ticks < floored_now_ticks) {
    fuzz_clock_ticks = floored_now_ticks;
    // sbi_printf("!!! fuzz_clock_ticks updated to %lu @ %lu, dif: %lu\n", fuzz_clock_ticks, now_ticks, now_ticks - prev_time);
    // prev_time = now_ticks;
  }
  
  // TODO(chungmcl): verify that sbi_sm_random is indeed uniformly random

  send_clock_ipi();

  return sbi_sm_random() % (granularity_ticks + 1);
#else
  // Fuzz Off
  uint64_t now_ticks = sbi_timer_value();
  fuzz_clock_ticks = now_ticks - (now_ticks % granularity_ticks);

  send_clock_ipi();

  return granularity_ticks;
#endif
}

void wait_for_ms(unsigned long ms) {
  sbi_timer_mdelay(ms);
}

unsigned long wait_until_epoch() {
  unsigned long current_time = fuzz_clock_ticks;
  while (current_time == fuzz_clock_ticks) { /* no-op */}
  return fuzz_clock_ticks;
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

/**
 * 
 *        Interprocessor Interrupts
 * 
 */
int get_clock_ipi_event_id() {
  return clock_ipi_event_id;
}

void handle_clock_ipi(struct sbi_scratch *scratch) {
  enclave_id eid = cpu_get_enclave_id() + 1;
  // sbi_printf("!!! handle_clock_ipi on core %d\n", current_hartid());

  if (cpu_is_enclave_context()) {
    // sbi_printf("\tcpu_is_enclave_context == true && hart_id: %d\n", current_hartid());
    short is_enclave_registered = (clock_ipi_registered_enclaves >> eid) & 0b1;
    if (is_enclave_registered) {
      // sbi_printf("\tenclave %d on hart %d is registered!\n", eid, current_hartid());
      csr_set(CSR_MIP, MIP_SSIP);
    }
  }
}

void reg_clock_ipi() {
  enclave_id eid = cpu_get_enclave_id() + 1;
  clock_ipi_registered_enclaves = clock_ipi_registered_enclaves | (0b1 << eid);
  sbi_printf("\treg_clock_ipi called:\n\t\tmhartid: %d\n\t\teid: %d\n\t\t\n", current_hartid(), eid);
}

void send_clock_ipi() {
  ulong mask = 0;
  sbi_hsm_hart_interruptible_mask(sbi_domain_thishart_ptr(), 0, &mask);
  sbi_ipi_send_many(mask, 0, clock_ipi_event_id, NULL);
}