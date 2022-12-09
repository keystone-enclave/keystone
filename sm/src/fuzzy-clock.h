// Note: in sm/plat/generic/objects.mk, 
// add an entry for fuzzy-clock.o
#ifndef _FUZZY_CLOCK_H_
#define _FUZZY_CLOCK_H_

#define FUZZ_ON 1

unsigned long update_fuzzy_clock();
void fuzzy_clock_init();
void wait_for_ms(unsigned long ms);
unsigned long wait_until_epoch();
unsigned long get_time_ticks();
unsigned long get_granularity_ticks();
int get_clock_ipi_event_id();

void reg_clock_ipi();

#endif