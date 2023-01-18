#include <asm/csr.h>
#include "util/asm_helpers.h"

#ifdef USE_CALLEE
.text

// Here, the only registers of value are a0 through a7 -- we can
// clobber everything else.
encl_call_handler:
    .global encl_call_handler
    mv t3, a0
    la a0, encl_call_lock
    LOAD a0, (a0)
    beqz a0, .L_failed_ret

    jal spin_lock_user
    mv a0, t3

    // We can now claim the callee stack
    la sp, callee_stack_end
    addi sp, sp, -80

    // Store registers into a
    STORE a0, 0*REGBYTES(sp)
    STORE a1, 1*REGBYTES(sp)
    STORE a2, 2*REGBYTES(sp)
    STORE a3, 3*REGBYTES(sp)
    STORE a4, 4*REGBYTES(sp)
    STORE a5, 5*REGBYTES(sp)
    STORE a6, 6*REGBYTES(sp)
    STORE a7, 7*REGBYTES(sp)

    mv a0, sp
    jal encl_call_handler_c
    mv t3, a0
    beqz t3, .L_failed_unlock

    // We don't need to pop any other registers here,
    // everything necessary has been copied to userspace
    LOAD gp, 9*REGBYTES(sp)
    LOAD tp, 8*REGBYTES(sp)

    // Retrieve the kernel stack for this thread
    ld sp, 10*REGBYTES(sp)
    csrrw sp, sscratch, sp

    // Return to glibc
    mv a0, x0
    mv sp, t3
    sret

// Unlock and return to SM
.L_failed_unlock:
    la a0, encl_call_lock
    ld a0, (a0)
    jal spin_unlock_user
.L_failed_ret:
    addi a0, x0, -1
    j sbi_ret_enclave
#endif // USE_CALLEE