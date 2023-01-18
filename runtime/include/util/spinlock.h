#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

#include <stdint.h>

#if __riscv_xlen == 64
typedef unsigned long spin_lock_t;
#elif __riscv_xlen == 32
typedef unsigned int spin_lock_t ;
#endif

void spin_lock(spin_lock_t *lock);
void spin_unlock(spin_lock_t *lock);
void spin_lock_user(spin_lock_t *lock);
void spin_unlock_user(spin_lock_t *lock);

#endif  // _SPINLOCK_H_
