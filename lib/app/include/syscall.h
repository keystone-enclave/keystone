#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <stdint.h>
#define SYSCALL_OCALL 1001
#define SYSCALL_UNTRUSTED_MMAP  1002
#define SYSCALL_EXIT  1101

#define SYSCALL(which, arg0, arg1, arg2) ( {\
	register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0);	\
	register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1);	\
	register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2);	\
	register uintptr_t a7 asm ("a7") = (uintptr_t)(which);	\
	asm volatile ("ecall"					\
		      : "+r" (a0)				\
		      : "r" (a1), "r" (a2), "r" (a7)		\
		      : "memory");				\
	a0;							\
})

#define SYSCALL_0(which) SYSCALL(which, 0, 0, 0)
#define SYSCALL_1(which, arg0) SYSCALL(which, arg0, 0, 0)
#define SYSCALL_2(which, arg0, arg1) SYSCALL(which, arg0, arg1, 0)

uintptr_t ocall(uintptr_t request);
uintptr_t untrusted_mmap();

#endif /* syscall.h */
