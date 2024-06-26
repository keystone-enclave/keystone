
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

// No special data needed for default platform
struct platform_enclave_data{

};

// Enclave configuration
#define ENCL_MAX                16
#define ENCLAVE_REGIONS_MAX     8

// SM configuration
// todo dont think this is correct
#define SMM_BASE                0x80000000
#define SMM_SIZE                0x200000

// PMP configuration
#define PMP_N_REG               16
#define PMP_MAX_N_REGION        16

// CPU configuration
#define MAX_HARTS               5

// Device configuration
#define DEV_NAMELEN             64
#define NUM_SECURE_DEVS         3
#define NUM_NONSECURE_DEVS      0
#define NUM_DISABLED_DEVS       0

// Search for FPGA devices
#define DEV_SEARCH_BASE         "/fabric-bus@40000000"
#define DEV_IGNORED             { "microchip,mpfs-ccc", "microchip,mpfs-qspi" }

// Initialization functions
void sm_copy_key(void);

#endif // _PLATFORM_H_
