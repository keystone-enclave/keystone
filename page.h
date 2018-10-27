#ifndef _SM_PAGE_H_
#define _SM_PAGE_H_

#include "bits.h"
#include "encoding.h"
#include "vm.h"
#include "mtrap.h" 

int init_encl_pgtable(int level, pte_t* pte, uintptr_t base, uintptr_t size);
//int print_pgtable(int level, pte_t* pte,uintptr_t vaddr);

#endif
