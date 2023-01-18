#include <asm/csr.h>
#include "util/asm_helpers.h"

/* spinlock.S includes assembly implementations of basic spinlocks which
 * closely mimic the implementations in both the SM and glibc. Of note is
 * that these spinlocks are offered as both assembly macros with parameterizable
 * register usage, as well as functions following the standard RISCV ABI. Of
 * note is that all implementations use no more than one argument register and
 * one temporary register, and do not rely on the stack at all. This makes them
 * very useful for locking in constrained scenarios, such as interrupt or
 * call handlers.
 */

.text

// Macros, for use inline
.macro _ENABLE_USER temp
    li \temp, SR_SUM
    csrs sstatus, \temp
.endm

.macro _DISABLE_USER temp
    li \temp, SR_SUM
    csrc sstatus, \temp
.endm

.macro SPIN_LOCK lock,temp
// First, check the lock (fast path)
1:
    LOAD \temp, 0(\lock)
    addi \temp, \temp, -1
    beqz \temp, 1b

// Try to actually acquire the lock
    addi \temp, \temp, 2
    SWAP \temp, \temp, (\lock)
    fence r,rw
    sext.w \temp, \temp
    bnez \temp, 1b
.endm

.macro SPIN_UNLOCK lock
    fence iorw,ow
    SWAP x0, x0, (\lock)
.endm

.macro SPIN_LOCK_USER lock, temp
    _ENABLE_USER \temp
    SPIN_LOCK \lock,\temp
    _DISABLE_USER \temp
.endm

.macro SPIN_UNLOCK_USER lock, temp
    _ENABLE_USER \temp
    SPIN_UNLOCK \lock
    _DISABLE_USER \temp
.endm

#ifdef SPINLOCK_MACROS_ONLY

.macro SPIN_LOCK_DECLARE name
\name:
    .global \name
    WORD 0
.endm

#else
spin_lock:
    .global spin_lock
    SPIN_LOCK a0, t6
    ret

spin_unlock:
    .global spin_unlock
    SPIN_UNLOCK a0
    ret

spin_lock_user:
    .global spin_lock_user
    SPIN_LOCK_USER a0, t6
    ret

spin_unlock_user:
    .global spin_unlock_user
    SPIN_UNLOCK_USER a0, t6
    ret
#endif