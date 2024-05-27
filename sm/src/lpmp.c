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
#include "sm-sbi-opensbi.h"
#include "lpmp.h"
#include "cpu.h"
#include "page.h"
#include "ipi.h"

static spinlock_t region_lock = SPIN_LOCK_INITIALIZER;
static lpmp_region_t host_regions[MAX_HOST_REGION_N];

static struct {
    struct sbi_dlist head;
    uint64_t count;
} host_reg;

static void __dump_host_region_list(void)
{
	lpmp_region_t *cur;
	int i = 0;
	uint64_t size = 0;
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

static inline void set_region(lpmp_region_t *reg, uintptr_t pa, uint64_t size)
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

static int __host_add_new_region(uintptr_t pa, uint64_t size, int is_inst)
{
	lpmp_region_t *reg = __host_alloc_new_region();
	reg->pa = pa;
	reg->size = size;
    reg->is_inst = is_inst;
	sbi_list_add_tail(&reg->entry, &host_reg.head);
	host_reg.count++;
    return 1;
}

static int __host_expand_region(uintptr_t pa, uint64_t size)
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

static int __host_add_region(uintptr_t pa, uint64_t size, int is_inst)
{
    int ret = __host_expand_region(pa, size);  // -1 (not expand), 0 (expand)
	if (ret)
		ret = __host_add_new_region(pa, size, is_inst);  // return #add_region
    return ret;
}

int host_add_region(uintptr_t pa, uint64_t size, int is_inst)
{
    spin_lock(&region_lock);
    int ret = __host_add_region(pa, size, is_inst);
    spin_unlock(&region_lock);
    return ret;  // return 0 or 1 (#added_region)
}

static void __host_split_region(uint64_t pa, uint64_t size, int is_inst)
{
    lpmp_region_t *cur;
    sbi_list_for_each_entry(cur, &host_reg.head, entry) {
		if (cur->pa <= pa && pa < cur->pa + cur->size) {
			uint64_t start_addr = cur->pa;
            uint64_t end_addr = start_addr + cur->size;
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

void host_split_region(uint64_t pa, uint64_t size, int is_inst)
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

int host_hit_region(uintptr_t pa)
{
    spin_lock(&region_lock);
    int ret = __host_hit_region(pa);
    spin_unlock(&region_lock);
    return ret;  // ret: 1 (hit), 0 (miss)
}

void host_free_regions(void)
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

static bool is_power_of_two(uint64_t num)
{
	if (!num)
		return 0;

	return (num & (num - 1)) == 0;
}

static int napot_power(uintptr_t pa, uint64_t size)
{
	if (is_power_of_two(size) && (pa % size == 0))
		return sbi_ffs(size);
	return -1;
}

static int set_pmp(int index, uintptr_t pa, uint64_t size)
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
        uint64_t size = cur->size;
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
    uint64_t prot, addr, log2len;

	smp_mb();

    for (i = 0; i < 8; i++) {
        rc = pmp_get(i, &prot, &addr, &log2len);
        if (rc) {
            sbi_panic("PMP info read error!\n");
        }
		char *pmpmode = ((prot & PMP_A) == 0) 			? "Off" : 
						((prot & PMP_A) == PMP_A_NA4) 	? "NA4" :
						((prot & PMP_A) == PMP_A_NAPOT) ? "NAPOT" :
						((prot & PMP_A) == PMP_A_TOR) 	? "TOR" :
						"Error";
		char *perm_r = (prot & PMP_R) ? "R" : "-";
		char *perm_w = (prot & PMP_W) ? "W" : "-";
		char *perm_x = (prot & PMP_X) ? "X" : "-";
		sbi_printf("PMP %u: Mode: %s, Permission: %s%s%s, "
			"addr: 0x%lx, len: 0x%lx\n",
			i, pmpmode, perm_r, perm_w, perm_x,
			addr, 1UL << log2len);
    }

	smp_mb();
    spin_unlock(&region_lock);
}
