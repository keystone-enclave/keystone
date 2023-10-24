#pragma once

#define BIT(n) (1ul << (n))
#define MASK(n) (BIT(n) - 1ul)

#if __riscv_xlen == 64
#define VA_BITS 39
#define RISCV_PT_INDEX_BITS 9
#define RISCV_PT_LEVELS 3
#else // __riscv_xlen == 32
#define VA_BITS 32
#define RISCV_PT_INDEX_BITS 10
#define RISCV_PT_LEVELS 2
#endif

#define RISCV_PAGE_BITS 12
#define RISCV_PAGE_SIZE (1 << RISCV_PAGE_BITS)
#define RISCV_PAGE_OFFSET(addr) (addr % RISCV_PAGE_SIZE)
#define RISCV_GET_PT_INDEX(addr, n)                                            \
  (((addr) >>                                                                  \
    (((RISCV_PT_INDEX_BITS) * ((RISCV_PT_LEVELS) - (n))) + RISCV_PAGE_BITS)) & \
   MASK(RISCV_PT_INDEX_BITS))
#define RISCV_GET_LVL_PGSIZE_BITS(n) \
  (((RISCV_PT_INDEX_BITS) * (RISCV_PT_LEVELS - (n))) + RISCV_PAGE_BITS)
#define RISCV_GET_LVL_PGSIZE(n) BIT(RISCV_GET_LVL_PGSIZE_BITS((n)))

#define ROUND_UP(n, b) (((((n)-1ul) >> (b)) + 1ul) << (b))
#define ROUND_DOWN(n, b) (n & ~((2 << (b - 1)) - 1))
#define PAGE_DOWN(n) ROUND_DOWN(n, RISCV_PAGE_BITS)
#define PAGE_UP(n) ROUND_UP(n, RISCV_PAGE_BITS)
#define MEGAPAGE_DOWN(n) ROUND_DOWN(n, RISCV_GET_LVL_PGSIZE_BITS(2))
#define MEGAPAGE_UP(n) ROUND_UP(n, RISCV_GET_LVL_PGSIZE_BITS(2))

#define IS_ALIGNED(x, align) (!((x) & (align - 1)))

/* Starting address of the enclave memory */

#if __riscv_xlen == 64
#define RUNTIME_VA_START 0xffffffffc0000000
#define EYRIE_LOAD_START 0xffffffff00000000
#define EYRIE_PAGING_START 0xffffffff40000000
#define EYRIE_UNTRUSTED_START 0xffffffff80000000
#define EYRIE_USER_STACK_START 0x0000000040000000
#define EYRIE_ANON_REGION_START \
  0x0000002000000000  // Arbitrary VA to start looking for large mappings
#else //__riscv_xlen == 32
#define RUNTIME_VA_START 0xc0000000 
#define EYRIE_LOAD_START 0xf0000000
#define EYRIE_PAGING_START 0x40000000
#define EYRIE_UNTRUSTED_START 0x80000000
#define EYRIE_USER_STACK_START 0x40000000
#define EYRIE_ANON_REGION_START \
  0x20000000  // Arbitrary VA to start looking for large mappings
#endif

#define EYRIE_ANON_REGION_END EYRIE_LOAD_START
#define EYRIE_USER_STACK_SIZE 0x20000
#define EYRIE_USER_STACK_END (EYRIE_USER_STACK_START - EYRIE_USER_STACK_SIZE)

#define PTE_V 0x001  // Valid
#define PTE_R 0x002  // Read
#define PTE_W 0x004  // Write
#define PTE_X 0x008  // Execute
#define PTE_U 0x010  // User
#define PTE_G 0x020  // Global
#define PTE_A 0x040  // Accessed
#define PTE_D 0x080  // Dirty
#define PTE_FLAG_MASK 0x3ff
#define PTE_PPN_SHIFT 10

typedef uintptr_t pte;
