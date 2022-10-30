// Note: in sm/plat/generic/objects.mk, 
// add an entry for fuzzy-time.o
#ifndef _FUZZY_TIME_H_
#define _FUZZY_TIME_H_

void fuzzy_time_init();
void fix_time_interval(unsigned long t);
void wait_for_ms(unsigned long ms);
unsigned long wait_until_epoch();
unsigned long get_time_ticks();
unsigned long get_granularity_ticks();

#endif