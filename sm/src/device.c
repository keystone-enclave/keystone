#include "device.h"
#include "sbi/sbi_console.h"
#include "sbi/riscv_asm.h"

#include <libfdt.h>
#include <sbi/sbi_string.h>

#ifndef TARGET_PLATFORM_HEADER
#error "SM requires a defined platform to build"
#endif

// Special target platform header, set by configure script
#include TARGET_PLATFORM_HEADER

#define IS_SECURE_DEVICE \
    (!strcmp(status, "disabled") && !strcmp(secure_status, "okay"))

#define IS_NONSECURE_DEVICE \
    (!strcmp(status, "okay") && !strcmp(secure_status, "disabled"))

#define IS_DISABLED_DEVICE \
    (!strcmp(status, "disabled") && !strcmp(secure_status, "disabled"))

// By default, search the device tree at /soc (can be overriden)
#ifndef DEV_SEARCH_BASE
#define DEV_SEARCH_BASE "/soc"
#endif

// Ignore some compatibles if requested by the platform
#ifdef DEV_IGNORED
#include <sbi/sbi_types.h>
static const char *ignored_compatibles[] = DEV_IGNORED;
#endif

struct keystone_device
{
    char devname[DEV_NAMELEN];
    region_id region_id;
    bool claimed;
};

/* device handling */
static int num_secure_devs = 0;
static int num_nonsecure_devs = 0;
static int num_disabled_devs = 0;

#if NUM_SECURE_DEVS
static struct keystone_device secure_devs[NUM_SECURE_DEVS];
#endif // NUM_SECURE_DEVS

#if NUM_NONSECURE_DEVS
static struct keystone_device nonsecure_devs[NUM_NONSECURE_DEVS];
#endif // NUM_NONSECURE_DEVS

#if NUM_DISABLED_DEVS
static struct keystone_device disabled_devs[NUM_DISABLED_DEVS];
#endif // NUM_NONSECURE_DEVS

int fdt_init_pmp_devices(void *fdt) {
    int i, ret, status_len, secure_status_len, name_len;
    const char *status, *secure_status, *name;
    const uint32_t *reg;

#ifdef DEV_IGNORED
    int compatible_len;
    const char *compatible;
#endif

    // Device tree variables
    int bus_offset, dev_offset, addr_cells, size_cells;

    // MMIO variables
    region_id region;
    uintptr_t mmio_addr;
    uint64_t mmio_size;

    // Clear the state
#if NUM_SECURE_DEVS
    memset(secure_devs, 0, sizeof(secure_devs));
#endif // NUM_SECURE_DEVS

#if NUM_NONSECURE_DEVS
    memset(nonsecure_devs, 0, sizeof(nonsecure_devs));
#endif // NUM_NONSECURE_DEVS

#if NUM_DISABLED_DEVS
    memset(disabled_devs, 0, sizeof(disabled_devs));
#endif // NUM_DISABLED_DEVS

    bus_offset = fdt_path_offset(fdt, DEV_SEARCH_BASE);

    // These return defaults if nothing is specified
    addr_cells = fdt_address_cells(fdt, bus_offset);
    size_cells = fdt_size_cells(fdt, bus_offset);

    fdt_for_each_subnode(dev_offset, fdt, bus_offset) {
#ifdef DEV_IGNORED
        // Check if we should ignore this device
        compatible = fdt_getprop(fdt, dev_offset, "compatible", &compatible_len);
        if(compatible) {
            // A device without a compatible string would not be accesible by Linux, since these strings
            // determine what driver to load. To that extent, it seems likely that such devices would
            // potentially be mapped by an enclave. Therefore, if there is no compatible string, we
            // choose to continue parsing in case it is a device we need to handle

            for(i = 0; i < array_size(ignored_compatibles); i++) {
                if(!strcmp(compatible, ignored_compatibles[i])) {
                        break;
                }
            }

            if(i != array_size(ignored_compatibles))
                continue;
        }
#endif

        // We follow the spec for secure nodes defined in the Linux kernel,
        // under Documentation/devicetree/bindings/arm/secure.txt.
        status = fdt_getprop(fdt, dev_offset, "status", &status_len);
        if(!status)
            status = "okay";

        secure_status = fdt_getprop(fdt, dev_offset, "secure-status", &secure_status_len);
        if(!secure_status)
            secure_status = status;

        // We only care about devices which are explicitly mapped into one world
        // (secure or nonsecure) or no worlds at all (fully disabled). The vast
        // majority of devices do not have status specified, which means that they
        // are enabled in both the secure and nonsecure worlds and we do not
        // specifically need to manage them through PMP.

        if(IS_SECURE_DEVICE || IS_NONSECURE_DEVICE || IS_DISABLED_DEVICE) {
            // This is a node which we need to register
            name = fdt_get_name(fdt, dev_offset, &name_len);
            if(!name || name_len >= DEV_NAMELEN) {
                goto _fail;
            }

            // Get the associated MMIO addresses
            reg = fdt_getprop(fdt, dev_offset, "reg", &ret);

            // We only support one MMIO region per device for now, with the standard
            // address-cells and size-cells specified at the root of the fdt. Otherwise,
            // things get a bit more complicated than OpenSBI's FDT library is designed for.
            if(!reg || ret != 4 * (addr_cells + size_cells)) {
                sbi_printf("[SM] device %s has more than one MMIO region, which is currently unsupported\n", name);
                continue;
            }

            mmio_addr = 0;
            for(i = 0; i < addr_cells; i++) {
                mmio_addr |= (fdt32_to_cpu(reg[i])
                        << (32 * (addr_cells - i - 1)));
            }

            mmio_size = 0;
            for(i = 0; i < size_cells; i++) {
                mmio_size |= (fdt32_to_cpu(reg[addr_cells + i])
                        << (32 * (size_cells - i - 1)));
            }

            ret = pmp_region_init_atomic(mmio_addr, ROUNDUP(mmio_size, PAGE_SIZE),
                                         PMP_PRI_ANY, &region, 0);
            if(ret) {
                goto _fail;
            }

            if(IS_SECURE_DEVICE) {
                if(num_secure_devs >= NUM_SECURE_DEVS) {
                    sbi_printf("[SM] too many secure devices specified\n");
                    goto _fail;
                }

#if NUM_SECURE_DEVS
                strcpy(secure_devs[num_secure_devs].devname, name);
                secure_devs[num_secure_devs].region_id = region;
                num_secure_devs++;
#endif // NUM_SECURE_DEVS
            } else if(IS_NONSECURE_DEVICE) {
                if(num_nonsecure_devs >= NUM_NONSECURE_DEVS) {
                    sbi_printf("[SM] too many nonsecure devices specified\n");
                    goto _fail;
                }

#if NUM_NONSECURE_DEVS
                strcpy(nonsecure_devs[num_nonsecure_devs].devname, name);
                nonsecure_devs[num_nonsecure_devs].region_id = region;
                num_nonsecure_devs++;
#endif // NUM_NONSECURE_DEVS
            } else if(IS_DISABLED_DEVICE) {
                if(num_disabled_devs >= NUM_DISABLED_DEVS) {
                    sbi_printf("[SM] too many disabled devices specified\n");
                    goto _fail;
                }

#if NUM_DISABLED_DEVS
                strcpy(disabled_devs[num_disabled_devs].devname, name);
                disabled_devs[num_disabled_devs].region_id = region;
                num_disabled_devs++;
#endif // NUM_DISABLED_DEVS
            }
        }
    }

    return 0;

_fail:
    num_secure_devs = 0;
    return -1;
}


region_id sm_claim_secure_device(const char *devname)
{
    __attribute__((unused)) int i;
    if(!devname) {
        return -1;
    }

#if NUM_SECURE_DEVS
    for(i = 0; i < MIN(num_secure_devs, NUM_SECURE_DEVS); i++) {
        if(strcmp(secure_devs[i].devname, devname) == 0) {
            if(!secure_devs[i].claimed) {
                secure_devs[i].claimed = true;
                return secure_devs[i].region_id;
            }
        }
    }
#endif // NUM_SECURE_DEVS

    // no match
    return -1;
}

region_id sm_release_secure_device(const char *devname)
{
    __attribute__((unused)) int i;
    if(!devname) {
        return -1;
    }

#if NUM_SECURE_DEVS
    for(i = 0; i < MIN(num_secure_devs, NUM_SECURE_DEVS); i++) {
        if(strcmp(secure_devs[i].devname, devname) == 0) {
            if(secure_devs[i].claimed) {
                secure_devs[i].claimed = false;
                return 0;
            } else {
                sbi_printf("[SM] attempting to release unclaimed secure device\n");
                return -1;
            }
        }
    }
#endif // NUM_SECURE_DEVS

    // no match
    return -1;
}

int device_switch_to_enclave(void) {
    // Here, we need to only disable all nonsecure devices. Enclaves will then
    // specifically enable the secure devices which they have claimed.
    __attribute__((unused)) int i, ret;

#if NUM_NONSECURE_DEVS
    for(i = 0; i < MIN(num_nonsecure_devs, NUM_NONSECURE_DEVS); i++) {
        ret = pmp_set_keystone(nonsecure_devs[i].region_id, PMP_NO_PERM);
        if(ret != SBI_ERR_SM_PMP_SUCCESS) {
            return -1;
        }
    }
#endif // NUM_NONSECURE_DEVS

#if NUM_DISABLED_DEVS
    // Ensure disable devices are still disabled
    for(i = 0; i < MIN(num_disabled_devs, NUM_DISABLED_DEVS); i++) {
        ret = pmp_set_keystone(disabled_devs[i].region_id, PMP_NO_PERM);
        if(ret != SBI_ERR_SM_PMP_SUCCESS) {
            return -1;
        }
    }
#endif // NUM_DISABLED_DEVS

    return 0;
}

int device_switch_to_host(void) {
    // Here, we need to disable all secure devices and enable all nonsecure
    // devices. Some of these secure devices will be re-disabled by functions
    // such as context_switch_to_host, but this is okay.
    __attribute__((unused)) int i, ret;

#if NUM_SECURE_DEVS
    for(i = 0; i < MIN(num_secure_devs, NUM_SECURE_DEVS); i++) {
        ret = pmp_set_keystone(secure_devs[i].region_id, PMP_NO_PERM);
        if(ret != SBI_ERR_SM_PMP_SUCCESS) {
            return -1;
        }
    }
#endif // NUM_SECURE_DEVS

#if NUM_NONSECURE_DEVS
    for(i = 0; i < MIN(num_nonsecure_devs, NUM_NONSECURE_DEVS); i++) {
        ret = pmp_set_keystone(nonsecure_devs[i].region_id, PMP_ALL_PERM);
        if(ret != SBI_ERR_SM_PMP_SUCCESS) {
            return -1;
        }
    }
#endif // NUM_NONSECURE_DEVS

    // Ensure disable devices are still disabled
#if NUM_DISABLED_DEVS
    for(i = 0; i < MIN(num_disabled_devs, NUM_DISABLED_DEVS); i++) {
        ret = pmp_set_keystone(disabled_devs[i].region_id, PMP_NO_PERM);
        if(ret != SBI_ERR_SM_PMP_SUCCESS) {
            return -1;
        }
    }
#endif // NUM_DISABLED_DEVS

    return 0;
}

const char *device_name_from_region(region_id rid) {
    __attribute__((unused)) int i;

#if NUM_SECURE_DEVS
    for(i = 0; i < MIN(num_secure_devs, NUM_SECURE_DEVS); i++) {
        if(secure_devs[i].region_id == rid) {
            return secure_devs[i].devname;
        }
    }
#endif // NUM_SECURE_DEVS

    return NULL;
}

region_id region_from_device_name(const char *devname) {
    __attribute__((unused)) int i;
    if(!devname) {
        return -1;
    }

#if NUM_SECURE_DEVS
    for(i = 0; i < MIN(num_secure_devs, NUM_SECURE_DEVS); i++) {
        if(strcmp(secure_devs[i].devname, devname) == 0) {
            return secure_devs[i].region_id;
        }
    }
#endif // NUM_SECURE_DEVS

    return -1;
}
