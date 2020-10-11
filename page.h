//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _SM_PAGE_H_
#define _SM_PAGE_H_

#include <sbi/sbi_bits.h>
#include <sbi/riscv_encoding.h>

#if __riscv_xlen == 64
# define RISCV_PGLEVEL_MASK 0x1ff
# define RISCV_PGTABLE_HIGHEST_BIT 0x100
#else
# define RISCV_PGLEVEL_MASK 0x3ff
# define RISCV_PGTABLE_HIGHEST_BIT 0x300
#endif

#define VA_BITS 39
#define RISCV_PGLEVEL_TOP ((VA_BITS - RISCV_PGSHIFT)/RISCV_PGLEVEL_BITS)
#endif
