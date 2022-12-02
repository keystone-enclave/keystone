//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <stddef.h>
#include <stdint.h>
#include "sealing.h"
/* TODO We should be syncing these more explictly with the runtime
   defs */
#define SYSCALL_OCALL 1001
#define SYSCALL_SHAREDCOPY 1002
#define SYSCALL_ATTEST_ENCLAVE 1003
#define SYSCALL_GET_SEALING_KEY 1004
// chungmcl
#define SYSCALL_SHAREDWRITE 1005
#define SYSCALL_PAUSE_MS 1006
#define SYSCALL_REG_CLOCK_IPI 1007
#define SYSCALL_PRINT_TIME 1008
// chungmcl
#define SYSCALL_EXIT 1101

#define SYSCALL(which, arg0, arg1, arg2, arg3, arg4)           \
  ({                                                           \
    register uintptr_t a0 asm("a0") = (uintptr_t)(arg0);       \
    register uintptr_t a1 asm("a1") = (uintptr_t)(arg1);       \
    register uintptr_t a2 asm("a2") = (uintptr_t)(arg2);       \
    register uintptr_t a3 asm("a3") = (uintptr_t)(arg3);       \
    register uintptr_t a4 asm("a4") = (uintptr_t)(arg4);       \
    register uintptr_t a7 asm("a7") = (uintptr_t)(which);      \
    asm volatile("ecall"                                       \
                 : "+r"(a0)                                    \
                 : "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a7) \
                 : "memory");                                  \
    a0;                                                        \
  })

#define SYSCALL_0(which) SYSCALL(which, 0, 0, 0, 0, 0)
#define SYSCALL_1(which, arg0) SYSCALL(which, arg0, 0, 0, 0, 0)
#define SYSCALL_2(which, arg0, arg1) SYSCALL(which, arg0, arg1, 0, 0, 0)
#define SYSCALL_3(which, arg0, arg1, arg2) \
  SYSCALL(which, arg0, arg1, arg2, 0, 0)
#define SYSCALL_4(which, arg0, arg1, arg2, arg3) \
  SYSCALL(which, arg0, arg1, arg2, arg3, 0)
#define SYSCALL_5(which, arg0, arg1, arg2, arg3, arg4) \
  SYSCALL(which, arg0, arg1, arg2, arg3, arg4)

int
copy_from_shared(void* dst, uintptr_t offset, size_t data_len);

int
ocall(
    unsigned long call_id, void* data, size_t data_len, void* return_buffer,
    size_t return_len);
uintptr_t
untrusted_mmap();
int
attest_enclave(void* report, void* data, size_t size);

int
get_sealing_key(
    struct sealing_key* sealing_key_struct, size_t sealing_key_struct_size,
    void* key_ident, size_t key_ident_size);

// chungmcl
/** Representation of per-HART scratch space */
struct sbi_scratch {
	/** Start (or base) address of firmware linked to OpenSBI library */
	unsigned long fw_start;
	/** Size (in bytes) of firmware linked to OpenSBI library */
	unsigned long fw_size;
	/** Arg1 (or 'a1' register) of next booting stage for this HART */
	unsigned long next_arg1;
	/** Address of next booting stage for this HART */
	unsigned long next_addr;
	/** Priviledge mode of next booting stage for this HART */
	unsigned long next_mode;
	/** Warm boot entry point address for this HART */
	unsigned long warmboot_addr;
	/** Address of sbi_platform */
	unsigned long platform_addr;
	/** Address of HART ID to sbi_scratch conversion function */
	unsigned long hartid_to_scratch;
	/** Address of trap exit function */
	unsigned long trap_exit;
	/** Temporary storage */
	unsigned long tmp0;
	/** Options for OpenSBI library */
	unsigned long options;
};

int
reg_clock_ipi(void (* process)(struct sbi_scratch *scratch));

int
write_to_shared(void* src, uintptr_t offset, size_t data_len);

int
pause_ms(unsigned long ms);

int
print_time();
// chungmcl

#endif /* syscall.h */
