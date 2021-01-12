//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _SM_PAGE_H_
#define _SM_PAGE_H_

#include <sbi/sbi_types.h>
#include <sbi/riscv_encoding.h>
#include <sbi/riscv_asm.h>

#define RISCV_PGSIZE  PAGE_SIZE
#define RISCV_PGSHIFT  PAGE_SHIFT

/* page table entry (PTE) fields */
#define PTE_V                _UL(0x001) /* Valid */
#define PTE_R                _UL(0x002) /* Read */
#define PTE_W                _UL(0x004) /* Write */
#define PTE_X                _UL(0x008) /* Execute */
#define PTE_U                _UL(0x010) /* User */
#define PTE_G                _UL(0x020) /* Global */
#define PTE_A                _UL(0x040) /* Accessed */
#define PTE_D                _UL(0x080) /* Dirty */
#define PTE_SOFT            _UL(0x300) /* Reserved for Software */

#if __riscv_xlen == 64
# define RISCV_PGLEVEL_MASK 0x1ff
# define RISCV_PGTABLE_HIGHEST_BIT 0x100
# define RISCV_PGLEVEL_BITS 9
#else
# define RISCV_PGLEVEL_MASK 0x3ff
# define RISCV_PGTABLE_HIGHEST_BIT 0x300
# define RISCV_PGLEVEL_BITS 10
#endif

#define PTE_PPN_SHIFT 10

#define VA_BITS 39
#define RISCV_PGLEVEL_TOP ((VA_BITS - RISCV_PGSHIFT)/RISCV_PGLEVEL_BITS)
#endif

