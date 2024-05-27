#ifndef _LPMP_H_
#define _LPMP_H_
#include "sm.h"
#include <sbi/riscv_atomic.h>
#include <sbi/sbi_list.h>

#define IDX_TO_ADDR_CSR(n) (CSR_PMPADDR0 + (n))

#if __riscv_xlen == 64
#define IDX_TO_CFG_CSR(n) ((CSR_PMPCFG0 + ((n) >> 2)) & ~1)
#define IDX_TO_CFG_SHIFT(n) (((n)&7) << 3)
#elif __riscv_xlen == 32
#define IDX_TO_CFG_CSR(n) (CSR_PMPCFG0 + ((n) >> 2))
#define IDX_TO_CFG_SHIFT(n) (((n)&3) << 3)
#else
#error "Unexpected __riscv_xlen"
#endif

#define PMP_A_OFF 0
#define PMP_ALL_PERM  (PMP_W | PMP_X | PMP_R)
#define PMP_NO_PERM   0
#define PMP_SET_IDX(idx, addr, perm, TYPE)                                 \
    do {                                                                   \
        int pmpaddr_csr = IDX_TO_ADDR_CSR(idx);                            \
        int pmpcfg_csr = IDX_TO_CFG_CSR(idx);                              \
        int pmpcfg_shift = IDX_TO_CFG_SHIFT(idx);                          \
        u64 cfg_mask = ~(0xFFUL << pmpcfg_shift);                          \
        u64 pmpcfg = csr_read_num(pmpcfg_csr) & cfg_mask;                  \
        pmpcfg |= ((perm | PMP_A_##TYPE) << pmpcfg_shift) & ~cfg_mask;     \
        csr_write_num(pmpaddr_csr, ((addr) >> PMP_SHIFT) & PMP_ADDR_MASK); \
        csr_write_num(pmpcfg_csr, pmpcfg);                                 \
    } while (0)

#define MAX_HOST_REGION_N  128  // can be larger

typedef struct {
	uintptr_t pa;
	uint64_t	size;
    int     is_inst;
	uint64_t	count;
    struct sbi_dlist entry;
} lpmp_region_t;

typedef int region_id;

void host_regions_init(void);

int host_add_region(uintptr_t pa, uint64_t size, int is_inst);

void host_split_region(uint64_t pa, uint64_t size, int is_inst);

int host_hit_region(uintptr_t pa);

void activate_host_lpmp(void);

void dump_host_region_list(void);

void pmp_clear(void);

void pmp_dump(void);

#endif
