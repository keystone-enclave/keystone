#include <sbi/sbi_timer.h>
#include "clock.h"
#include "enclave.h"
#include "cpu.h"
#include <sbi/sbi_console.h> // for sbi_printf() for debugging
#include "string.h"

#include "sbi/sbi_ipi.h"
#include "sbi/sbi_hsm.h"
#include "sbi/sbi_domain.h"

#define GRANULARITY_MS 10
unsigned long granularity_ticks;
unsigned long ticks_per_ms;

volatile unsigned long fuzz_clock_ticks;

// bitmask of enclaves registered for fuzzy clock IPIs
uint64_t clock_ipi_registered_enclaves; 
int clock_ipi_event_id = -1;
static struct sbi_ipi_event_ops clock_ipi_event_ops;

// Private functions
void handle_clock_ipi(struct sbi_scratch *scratch);
void send_clock_ipi();

void clock_init() {
  // (ticks / second) * (seconds / milliseconds) = (ticks / milliseconds)
  ticks_per_ms = sbi_timer_get_device()->timer_freq / 1000;
  // milliseconds * (ticks / milliseconds) = ticks
  granularity_ticks = GRANULARITY_MS * ticks_per_ms;
  clock_ipi_registered_enclaves = 0;
  clock_ipi_event_ops.name[0] = 'x';
  clock_ipi_event_ops.name[1] = '\0';
  clock_ipi_event_ops.process = handle_clock_ipi;
  clock_ipi_event_id = sbi_ipi_event_create(&clock_ipi_event_ops);

  sbi_printf("[SM] Fuzzy Clock IPI event created with ID: %d\n", clock_ipi_event_id);
  sbi_printf("[SM] Fuzzy Clock initialized with granularity_ticks == %lu\n", granularity_ticks);
}

unsigned long update_fuzzy_clock() {
  uint64_t now_ticks = sbi_timer_value();
  
  uint64_t floored_now_ticks = now_ticks - (now_ticks % granularity_ticks);
  if (fuzz_clock_ticks < floored_now_ticks) {
    fuzz_clock_ticks = floored_now_ticks;
  }
  
  send_clock_ipi();

  // TODO(chungmcl): verify that sbi_sm_random is indeed uniformly random
  return sbi_sm_random() % (granularity_ticks + 1);
}

void wait_for_ms(unsigned long ms) {
  sbi_timer_mdelay(ms);
}

unsigned long wait_until_epoch() {
  unsigned long current_time = fuzz_clock_ticks;
  while (current_time == fuzz_clock_ticks) { /* no-op */ }
  return fuzz_clock_ticks;
}

bool get_is_clock_fuzzy() {
  enclave_id eid = cpu_get_enclave_id();
  return (clock_ipi_registered_enclaves & (0b1 << eid)) >> eid;
}

unsigned long get_time_ticks() {
  if (get_is_clock_fuzzy()) {
    return fuzz_clock_ticks;
  } else {
    return sbi_timer_value();
  }
}

unsigned long get_granularity_ticks() {
  if (get_is_clock_fuzzy()) {
    return granularity_ticks;
  } else {
    return 0;
  }
}

/**
 * 
 *    Interprocessor Interrupts
 * 
 */
void reg_fuzzy_clock_ipi(enclave_id eid) {
  clock_ipi_registered_enclaves = clock_ipi_registered_enclaves | (0b1 << eid);
  sbi_printf("\treg_clock_ipi called:\n\t\tmhartid: %d\n\t\teid: %d\n\t\t\n", current_hartid(), eid);
}

int get_clock_ipi_event_id() {
  return clock_ipi_event_id;
}

void handle_clock_ipi(struct sbi_scratch *scratch) {
  enclave_id eid = cpu_get_enclave_id();

  if (cpu_is_enclave_context()) {
    uint64_t is_enclave_registered = (clock_ipi_registered_enclaves >> eid) & 0b1;
    if (is_enclave_registered) {
      // Trigger an S-Mode interrupt
      csr_set(CSR_MIP, MIP_SSIP);
    }
  }

  csr_clear(CSR_MIP, MIP_MSIP);
}

void send_clock_ipi() {
  ulong mask = 0;
  sbi_hsm_hart_interruptible_mask(sbi_domain_thishart_ptr(), 0, &mask);
  sbi_ipi_send_many(mask, 0, clock_ipi_event_id, NULL);
}