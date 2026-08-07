#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "pmp.h"

/* exported functions */

// initialization
int fdt_init_pmp_devices(void *fdt);

// control
region_id sm_claim_secure_device(const char *devname);
region_id sm_release_secure_device(const char *devname);

int device_switch_to_enclave(void);
int device_switch_to_host(void);

// internal utility. these are only implemented for secure devices
const char *device_name_from_region(region_id rid);
region_id region_from_device_name(const char *devname);

#endif //_DEVICE_H_
