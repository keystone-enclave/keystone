#include <sbi/riscv_asm.h>
#include <sbi/riscv_encoding.h>
#include <sbi/riscv_barrier.h>
#include <sbi/riscv_locks.h>
#include <sbi/riscv_atomic.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_string.h>
#include <sbi/sbi_bitops.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_list.h>
#include <sbi/sbi_types.h>
#include "sm-sbi-opensbi.h"
#include "lpmp.h"
#include "cpu.h"
#include "page.h"
#include "ipi.h"
#include "sm.h"

/* PMP defines */

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

static struct {
    struct sbi_dlist head;
    size_t count;
} host_reg;

typedef struct {
    uintptr_t pa;
    size_t size;
    int is_inst;
    size_t count;
    struct sbi_dlist entry;
} lpmp_region_t;

static spinlock_t region_lock = SPIN_LOCK_INITIALIZER;
static lpmp_region_t host_regions[MAX_HOST_REGION_N];
typedef int region_id;

/* Paging defines */

#define PPNSHIFT        9
#define PGSHIFT         12
#define PTE2PA(pte)     (((pte) >> 10) << 12)
#define PXMASK          0x1FF  // 9 bits
#define PXSHIFT(level)  (PGSHIFT + (9 * (level)))
#define PX(level, va)   ((((uintptr_t)(va)) >> PXSHIFT(level)) & PXMASK)

typedef uintptr_t *pagetable_t;
typedef uintptr_t pte_t;

static void __dump_host_region_list(void)
{
	lpmp_region_t *cur;
	int i = 0;
	size_t size = 0;
	sbi_printf("Dump region list of Host, count = %lu:\n",
		host_reg.count);

	if (!host_reg.count) {
		return;
	}
    sbi_list_for_each_entry(cur, &host_reg.head, entry) {
		sbi_printf("\tRegion %d: "
            "pa = 0x%lx, size = 0x%lx, is_inst: %d\n",
			i,
			cur->pa,
			cur->size,
            cur->is_inst);
		i++;
		size += cur->size;
		if (i > host_reg.count) {
			sbi_panic("Exceed max num");
		}
	}
	sbi_printf("Total size: 0x%lx\n", size);
	sbi_printf("------------------------------\n");
}

void dump_host_region_list(void)
{
	spin_lock(&region_lock);
	__dump_host_region_list();
	spin_unlock(&region_lock);
}

static inline void set_region(lpmp_region_t *reg, uintptr_t pa, size_t size)
{
	reg->pa 	= pa;
	reg->size 	= size;
}

static lpmp_region_t * __host_alloc_new_region(void)
{
	for (int i = 0; i < MAX_HOST_REGION_N; i++) {
		lpmp_region_t *reg = &host_regions[i];
		if (!reg->size) {
			reg->pa = 0;
			SBI_INIT_LIST_HEAD(&reg->entry);
			return reg;
		}
	}
	sbi_panic("Out of region source\n");
	return NULL;
}

static int __host_add_new_region(uintptr_t pa, size_t size, int is_inst)
{
	lpmp_region_t *reg = __host_alloc_new_region();
	reg->pa = pa;
	reg->size = size;
    reg->is_inst = is_inst;
	sbi_list_add_tail(&reg->entry, &host_reg.head);
	host_reg.count++;
    return 1;
}

static int __host_expand_region(uintptr_t pa, size_t size)
{
	lpmp_region_t *cur;
	sbi_list_for_each_entry(cur, &host_reg.head, entry) {
		uintptr_t start = -1UL;
		if (pa + size == cur->pa) { // left adjacent
			start = pa;
		} else if (cur->pa + cur->size == pa) { // right adjacent
			start = cur->pa;
		} else if (cur->pa == pa && cur->size == size) {
			return 0; // MMIO device
		}
		if (start != -1UL) {
			set_region(cur, start, size + cur->size);
			return 0;
		}
	}
	return -1;
}

static void __host_remove_region(uintptr_t pa)
{
	lpmp_region_t *cur;
	sbi_list_for_each_entry(cur, &host_reg.head, entry) {
		if (pa == cur->pa) {
			sbi_list_del_init(&cur->entry);
			host_reg.count--;
			return;
		}
	}
}

static int __host_add_region(uintptr_t pa, size_t size, int is_inst)
{
    int ret = __host_expand_region(pa, size);  // -1 (not expand), 0 (expand)
	if (ret)
		ret = __host_add_new_region(pa, size, is_inst);  // return #add_region
    return ret;
}

int host_add_region(uintptr_t pa, size_t size, int is_inst)
{
    spin_lock(&region_lock);
    int ret = __host_add_region(pa, size, is_inst);
    spin_unlock(&region_lock);
    return ret;  // return 0 or 1 (#added_region)
}

static void __host_split_region(uintptr_t pa, size_t size, int is_inst)
{
    lpmp_region_t *cur;
    sbi_list_for_each_entry(cur, &host_reg.head, entry) {
		if (cur->pa <= pa && pa < cur->pa + cur->size) {
			uintptr_t start_addr = cur->pa;
            uintptr_t end_addr = start_addr + cur->size;
            __host_remove_region(cur->pa);
            if (pa > start_addr)
                __host_add_region(start_addr, pa - start_addr, 0);
            if (pa + size < end_addr)
                __host_add_region(pa + size, end_addr - (pa + size), 0);
            return;
        }
    }
    return;
}

void host_split_region(uintptr_t pa, size_t size, int is_inst)
{
    spin_lock(&region_lock);
    __host_split_region(pa, size, is_inst);
    spin_unlock(&region_lock);
}

static int __host_hit_region(uintptr_t pa)
{
    lpmp_region_t *cur;
    sbi_list_for_each_entry(cur, &host_reg.head, entry) {
		if (cur->pa <= pa && pa < cur->pa + cur->size) {
			cur->count++;
			// Apply LRU for swapping LPMP.
			sbi_list_del_init(&cur->entry);
			sbi_list_add(&cur->entry, &host_reg.head);
			return 1;
		}
	}
    return 0;
}

static int host_hit_region(uintptr_t pa)
{
    spin_lock(&region_lock);
    int ret = __host_hit_region(pa);
    spin_unlock(&region_lock);
    return ret;  // ret: 1 (hit), 0 (miss)
}


__attribute__((unused))
static void host_free_regions(void)
{
    spin_lock(&region_lock);
    lpmp_region_t *cur;
    sbi_list_for_each_entry(cur, &host_reg.head, entry) {
        set_region(cur, 0, 0);
        cur->count = 0;
    }
    SBI_INIT_LIST_HEAD(&host_reg.head);
	spin_unlock(&region_lock);
}

void host_regions_init(void)
{
	sbi_memset(host_regions, 0, sizeof(host_regions));  // clear out
    SPIN_LOCK_INIT(region_lock);
    SBI_INIT_LIST_HEAD(&host_reg.head);
    host_reg.count = 0;
    host_add_region(0, SMM_BASE, 0);
    host_add_region(SMM_BASE+SMM_SIZE, -1UL-(SMM_BASE+SMM_SIZE), 0);
}

void pmp_clear(void)
{
    for (int i = 0; i < PMP_N_REG; ++i)
        PMP_SET_IDX(i, 0, PMP_NO_PERM, OFF);
}

static bool is_power_of_two(uintptr_t num)
{
	if (!num)
		return 0;

	return (num & (num - 1)) == 0;
}

static int napot_power(uintptr_t pa, size_t size)
{
	if (is_power_of_two(size) && (pa % size == 0))
		return sbi_ffs(size);
	return -1;
}

static int set_pmp(int index, uintptr_t pa, size_t size)
{
    int ret = 0;
    if (index < 0)
		return ret;
    int pow = napot_power(pa, size);
    if (pow != -1) {    // NAPOT
        if (index < 0)
            return ret;
        uintptr_t napot_addr = pa | ((1UL << (pow - 1)) - 1);
        PMP_SET_IDX(index, napot_addr, PMP_ALL_PERM, NAPOT);
        ret = 1;
    } else {            // TOR
        if (index < 1)
            return ret;
		PMP_SET_IDX(index - 1, pa, PMP_ALL_PERM, OFF);
		PMP_SET_IDX(index, pa + size, PMP_ALL_PERM, TOR);
        ret = 2;
    }

    return ret;
}

static void __activate_host_pmp(int pmp_count)
{
    int index = pmp_count - 1;
    lpmp_region_t *cur;
    sbi_list_for_each_entry(cur, &host_reg.head, entry) {
        uintptr_t pa = cur->pa;
        size_t size = cur->size;
        int consumed = set_pmp(index, pa, size);
        if (!consumed)
            break;
        index -= consumed;
    }
    return;
}

void activate_host_lpmp(void)
{
    spin_lock(&region_lock);
    pmp_clear();
    smp_mb();
    __activate_host_pmp(PMP_N_REG);

    spin_unlock(&region_lock);
}

void pmp_dump(void)
{
    spin_lock(&region_lock);
	unsigned int i;
    int rc;
    unsigned long prot, addr, log2len;

	smp_mb();

    for (i = 0; i < 8; i++) {
        rc = pmp_get(i, &prot, &addr, &log2len);
        if (rc) {
            sbi_panic("PMP info read error!\n");
        }
		const char *pmpmode = ((prot & PMP_A) == 0) 			? "Off" :
						((prot & PMP_A) == PMP_A_NA4) 	? "NA4" :
						((prot & PMP_A) == PMP_A_NAPOT) ? "NAPOT" :
						((prot & PMP_A) == PMP_A_TOR) 	? "TOR" :
						"Error";
		const char *perm_r = (prot & PMP_R) ? "R" : "-";
		const char *perm_w = (prot & PMP_W) ? "W" : "-";
		const char *perm_x = (prot & PMP_X) ? "X" : "-";
		sbi_printf("PMP %u: Mode: %s, Permission: %s%s%s, "
			"addr: 0x%lx, len: 0x%lx\n",
			i, pmpmode, perm_r, perm_w, perm_x,
			addr, 1UL << log2len);
    }

	smp_mb();
    spin_unlock(&region_lock);
}

static uintptr_t walkaddr(pagetable_t pagetable, uintptr_t va) {
    if (pagetable == 0)
        return 0;
    pte_t *pte;
    int level;
    uintptr_t pa;

    // make sure page tables in PMP.
    host_hit_region((uintptr_t)pagetable);
    for (level = 4; level > 0; level--) {
        pte = &pagetable[PX(level, va)];
        if (*pte & (PTE_X | PTE_W | PTE_R)) {
            goto found;  // A leaf pte has been found.
        } else if (*pte & PTE_V) {
            pagetable = (pagetable_t)PTE2PA(*pte);
            host_hit_region((uintptr_t)pagetable);
        } else {
            sbi_panic("invalid va=0x%lx\n", va);
        }
    }
    pte = &pagetable[PX(0, va)];

found:
    if (pte == 0)
        return 0;
    if ((*pte & PTE_V) == 0)
        return 0;
    uintptr_t number_of_ones = PGSHIFT + level * PPNSHIFT;
    uintptr_t offset_mask = (1 << number_of_ones) - 1;
    uintptr_t offset = (va & offset_mask);
    pa = PTE2PA(*pte) + offset;

    return pa;
}

static inline void flush_tlb(void)
{
         asm volatile("sfence.vma");
}

static uintptr_t get_pt_root(void) {
    return ((csr_read(satp) & 0xFFFFFFFFFFF) << 12);
}

int pmp_fault_handler(ulong mtval) {
    if (!mtval) {
        sbi_printf("mepc = 0x%lx\n", csr_read(CSR_MEPC));
        sbi_printf("Null pointer!\n");
        return -1;
    }
    pagetable_t pt_root = (pagetable_t)get_pt_root();
    uintptr_t pa = pt_root ? walkaddr(pt_root, mtval) : mtval;

    if (pa && host_hit_region(pa)) {
        activate_host_lpmp();
        // Option 1. enable TLB cached PMP.
        asm volatile("sfence.vma       %0, zero        \n\t" : : "r"(mtval));

        // Option 2. disable TLB cached PMP.
        // flush_tlb();

        return 0;
    } else {
        sbi_printf("Error: Host should not access this pa\n");
        return -1;
    }
}
