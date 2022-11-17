#include <sbi/sbi_timer.h>
#include "fuzzy-time.h"
#include "enclave.h"
#include "cpu.h"
#include <sbi/sbi_console.h> // for sbi_printf() for debugging

#define GRANULARITY_MS 10

unsigned long fuzz_clock_ticks;
unsigned long t_end_ticks = 0;
unsigned long granularity_ticks;
unsigned long ticks_per_ms;

void fuzzy_time_init() {
  // (ticks / second) * (seconds / milliseconds) = (ticks / milliseconds)
  ticks_per_ms = sbi_timer_get_device()->timer_freq / 1000;
  // milliseconds * (ticks / milliseconds) = ticks
  granularity_ticks = GRANULARITY_MS * ticks_per_ms;
  unsigned long t = sbi_timer_value();
  t_end_ticks = t - (t % granularity_ticks);
}

unsigned long prev_time;

unsigned long update_fuzzy_clock() {
  uint64_t now_ticks = sbi_timer_value();


  uint64_t floored_now_ticks = now_ticks - (now_ticks % granularity_ticks);
  if (fuzz_clock_ticks < floored_now_ticks) {
    fuzz_clock_ticks = floored_now_ticks;
    // sbi_printf("!!! fuzz_clock_ticks updated to %lu @ %lu, dif: %lu\n", fuzz_clock_ticks, now_ticks, now_ticks - prev_time);
    prev_time = now_ticks;
  }

  // TODO(chungmcl): verify that sbi_sm_random is indeed uniformly random
  return sbi_sm_random() % (granularity_ticks + 1);
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