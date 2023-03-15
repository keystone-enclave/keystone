// Note: in sm/plat/generic/objects.mk, 
// add an entry for clock.o
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "enclave.h"

void clock_init();
unsigned long update_fuzzy_clock();
void wait_for_ms(unsigned long ms);
unsigned long wait_until_epoch();
bool get_is_clock_fuzzy();
unsigned long get_time_ticks();
unsigned long get_granularity_ticks();

void reg_fuzzy_clock_ipi(enclave_id eid);

int get_clock_ipi_event_id();


#endif