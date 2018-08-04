#ifndef _PAGE_H_
#define _PAGE_H_

#include <stdint.h>
#include "vm.h"
#include "bits.h"

/* IMPORTANT: This code assumes Sv39 */
#define PAGE_MASK   0xfff
#define PTE_OFFSET_MASK 0x1ff
#define PTE_OFFSET_SIZE 9


#define PPN(pa) (pa>>PAGE_SHIFT);
#define VPN(va) (va>>PAGE_SHIFT);

#define PTE_FLAG_IS_SET(flag) (0x1 << flag)

#define VA_PTE_OFFSET   PAGE_SHIFT
#define VA_PMD_OFFSET   (VA_PTE_OFFSET+PTE_OFFSET_SIZE)
#define VA_PGD_OFFSET   (VA_PMD_OFFSET+PTE_OFFSET_SIZE)
#define VA_PGD(va)  ((va >> VA_PGD_OFFSET) & PTE_OFFSET_MASK)
#define VA_PMD(va)  ((va >> VA_PMD_OFFSET) & PTE_OFFSET_MASK)
#define VA_PTE(va)  ((va >> VA_PTE_OFFSET) & PTE_OFFSET_MASK)

typedef uintptr_t paddr_t;
typedef uintptr_t vaddr_t;

typedef struct pg_list_t
{
  paddr_t head;
  paddr_t tail;
  unsigned int count;
} pg_list_t;

//enclave private memory;
typedef struct epm_t {
  struct pg_list_t freelist;
  pte_t* root_page_table;
  paddr_t base;
  unsigned int total;
} epm_t;

uintptr_t inline epm_satp(epm_t* epm) {
  return ((uintptr_t)epm->root_page_table >> RISCV_PGSHIFT | SATP_MODE_CHOICE);
}
void init_free_pages(pg_list_t* pg_list, paddr_t base, unsigned int count);
void put_free_page(pg_list_t* pg_list, paddr_t page_addr);
paddr_t get_free_page(pg_list_t* pg_list);

void epm_init(epm_t* epm, paddr_t base, unsigned int count);

paddr_t epm_alloc_page(epm_t* epm, vaddr_t addr);
void epm_free_page(epm_t* epm, vaddr_t addr);
//pfn_t epm_alloc_pages(epm_t* epm, va_t va, int order);

#endif
