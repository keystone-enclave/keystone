// Note: in sm/plat/generic/objects.mk, 
// add an entry for clock.o
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "enclave.h"

/**
 * @brief Defines the granularity (time between epcohs) of the fuzzy clock in milliseconds.
 * 
 */
#define GRANULARITY_MS 10

/**
 * @brief Initialize the SM clock.
 * 
 */
void clock_init();

/**
 * @brief Move the fuzzy clock forward in time.
 * 
 * @return unsigned long: The next real time in ticks at which 
 * this function should be called again.
 */
unsigned long update_fuzzy_clock();

/**
 * @brief Wait a real time amount of milliseconds.
 * 
 * @param ms: number of milliseconds to wait.
 */
void wait_for_ms(unsigned long ms);

/**
 * @brief Wait until the next fuzzy clock epoch.
 * 
 * @return unsigned long: Current fuzzy clock value in ticks.
 */
unsigned long wait_until_epoch();

/**
 * @brief Check if the current enclave is using the fuzzy clock.
 * 
 * @return true 
 * @return false 
 */
bool get_is_clock_fuzzy();

/**
 * @brief Get the current time in ticks.
 * 
 * @return unsigned long: The current time, in ticks.
 * If the current enclave context has fuzzy clock enabled,
 * returns fuzzy clock time in ticks. Otherwise, returns real
 * time in ticks.
 */
unsigned long get_time_ticks();

/**
 * @brief Get the granularity (time between epochs) of the fuzzy clock in ticks.
 * 
 * @return unsigned long: The granularity of the fuzzy clock in ticks.
 */
unsigned long get_granularity_ticks();

/**
 * @brief Register a specified enclave for fuzzy clock inter-processor timer interrupts (IPI).
 * 
 * @param eid: The enclave id of the enclave to register.
 */
void reg_fuzzy_clock_ipi(enclave_id eid);

/**
 * @brief Unregister a specified enclave for fuzzy clock inter-processor timer interrupts (IPI).
 * 
 * @param eid: The enclave id of the enclave to Unregister.
 */
void unreg_fuzzy_clock_ipi(enclave_id eid);

/**
 * @brief Get the inter-processor timer interrupt (IPI) ID of the fuzzy clock inter-processor timer interrupt.
 * 
 * @return int: The inter-processor timer interrupt ID of the fuzzy clock inter-processor timer interrupt.
 */
int get_clock_ipi_event_id();


#endif