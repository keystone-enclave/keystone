#ifndef __VM_H__
#define __VM_H__

#define BIT(n) (1ul << (n))
#define MASK(n) (BIT(n)-1ul)

#define RISCV_PT_INDEX_BITS 9
#define RISCV_PT_LEVELS 3
#define RISCV_PAGE_BITS 12
#define RISCV_PAGE_SIZE (1<<RISCV_PAGE_BITS)
#define RISCV_GET_PT_INDEX(addr, n) \
  (((addr) >> (((RISCV_PT_INDEX_BITS) * ((RISCV_PT_LEVELS) - (n))) + RISCV_PAGE_BITS)) \
   & MASK(PT_INDEX_BITS))


typedef uintptr_t pte_t;
extern pte_t root_page_table[BIT(RISCV_PT_INDEX_BITS)];

#endif
