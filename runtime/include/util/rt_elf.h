#ifndef __RT_ELF_H__
#define __RT_ELF_H__

#include "util/elf.h"

/* Internal helper macros */
#define _strip(X) X
#define __elf(type, len) Elf##len##_##type
#define _elf(type, len) __elf(type, len)

/* Convenience macros so we don't have to ifdef on __riscv_xlen */
#define ELF(type) _elf(type, _strip(__riscv_xlen))

#endif  // __RT_ELF_H__
