#ifndef _PLATFORM_H_
#define _PLATFORM_H_

/* These functions are defined by platform/soc specific objects,
   defined in platform/$PLATFORM/$PLATFORM.c */

/* This fires once FOR EACH enclave during init of enclave
   metadata. It may not fail currently. */
void platform_init(struct platform_enclave_data_t* enclave);

/* This fires when context switching INTO an enclave from the OS */
void platform_switch_to_enclave(struct platform_enclave_data_t* enclave);

/* This fires when context switching OUT of an enclave into the OS */
void platform_switch_from_enclave(struct platform_enclave_data_t* enclave);

/* Future version: This fires when context switching from enclave A to
   enclave B */
// void platform_switch_between_enclaves(platform_enclave_data_t* enclaveA,
//                                       platform_enclave_data_t* enclaveB);

/* This is a required feature, it must return 64bits of random data on
   demand and never fail. If it would fail it may power off
   instead. */
uint64_t platform_random();
#endif /* _PLATFORM_H_ */
