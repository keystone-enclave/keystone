#ifndef _KEYSTONE_ABI_
#define _KEYSTONE_ABI_

#include <cstdint>

#define ABI_CALL(which, arg0, arg1, arg2) ({			\
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

/* Lazy implementations until ABI is finalized */
#define ABI_CALL_0(which) ABI_CALL(which, 0, 0, 0)
#define ABI_CALL_1(which, arg0) ABI_CALL(which, arg0, 0, 0)
#define ABI_CALL_2(which, arg0, arg1) ABI_CALL(which, arg0, arg1, 0)

#endif
