#ifndef _SAFE_MATH_UTIL_H_
#define _SAFE_MATH_UTIL_H_

// TODO: We may want to add checks for GCC/Clang version. We also want
// to add wrappers for other checked math functions.

#if __riscv_xlen == 32
#define CHECKED_ADD(a, b, out) (__builtin_uadd_overflow(a, b, (unsigned int*) out))
#else
#define CHECKED_ADD(a, b, out) (__builtin_uaddl_overflow(a, b, (unsigned long int*) out))
#endif

#endif /* _SAFE_MATH_UTIL_H_ */
