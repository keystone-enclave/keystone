#include "enclave.h"
#include "crypto.h"
#include "page.h"


int hash_epm(hash_ctx_t* hash_ctx, int level, pte_t* tb, uintptr_t vaddr, int contiguous)
{
  pte_t* walk;
  int i;

  /* iterate over PTEs */
  for (walk=tb, i=0; walk < tb + (RISCV_PGSIZE/sizeof(pte_t)); walk += 1,i++)
  {
    if (*walk == 0) {
      contiguous = 0;
      continue;
    }
    uintptr_t vpn;
    
    /* propagate the highest bit of the VA */
    if ( level == RISCV_PGLEVEL_TOP && i & RISCV_PGTABLE_HIGHEST_BIT )
      vpn = ((-1 << RISCV_PGLEVEL_BITS) | (i & RISCV_PGLEVEL_MASK));
    else
      vpn = ((vaddr << RISCV_PGLEVEL_BITS) | (i & RISCV_PGLEVEL_MASK));
    
    /* include the first virtual address of a contiguous range */
    if (level == 1 && !contiguous)
    {
      uintptr_t va_start = vpn << RISCV_PGSHIFT;
      hash_extend(hash_ctx, &va_start, sizeof(uintptr_t));
      printm("VA hashed: 0x%lx\n", va_start);
      contiguous = 1;
    }

    uintptr_t phys_addr = (*walk >> PTE_PPN_SHIFT) << RISCV_PGSHIFT;
    if (level == 1)
    {
      /* if PTE is leaf, extend hash for the page */
      hash_extend_page(hash_ctx, (void*)phys_addr);
      printm("PAGE hashed: 0x%lx (pa: 0x%lx)\n", vpn << RISCV_PGSHIFT, phys_addr); 
    }
    else
    {
      /* otherwise, recurse on a lower level */
      contiguous = hash_epm(hash_ctx, 
          level - 1, 
          (pte_t*) phys_addr, 
          vpn, 
          contiguous);
    }
  }

  return contiguous;
}

enclave_ret_t hash_enclave(struct enclave_t* enclave)
{
  enclave_ret_t ret;
  hash_ctx_t hash_ctx;
  int ptlevel = RISCV_PGLEVEL_TOP;
  int i;
  hash_init(&hash_ctx);

  hash_epm(&hash_ctx, 
      ptlevel, 
      (pte_t*) (enclave->encl_satp << RISCV_PGSHIFT),
      0, 0);
  
  hash_finalize(enclave->hash, &hash_ctx);
  printm("Final hash:\n");
  for(i = 0; i<MDSIZE/sizeof(uint64_t); i++)
  {
    printm("%lx\n", *((uint64_t*) enclave->hash + i));
  }
  return ENCLAVE_SUCCESS;
}
