//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "enclave.h"
#include "crypto.h"
#include "page.h"

/* This will walk the entire vaddr space in the enclave, validating
   linear at-most-once paddr mappings, and then hashing valid pages */
int validate_and_hash_epm(hash_ctx* hash_ctx, int level,
                          pte_t* tb, uintptr_t vaddr, int contiguous,
                          struct keystone_sbi_create* cargs,
                          uintptr_t* runtime_max_seen,
                          uintptr_t* user_max_seen)
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
    uintptr_t phys_addr = (*walk >> PTE_PPN_SHIFT) << RISCV_PGSHIFT;

    /* Check for blatently invalid mappings */
    int map_in_epm = (phys_addr >= cargs->epm_region.paddr &&
                      phys_addr < cargs->epm_region.paddr + cargs->epm_region.size);
    int map_in_utm = (phys_addr >= cargs->utm_region.paddr &&
                      phys_addr < cargs->utm_region.paddr + cargs->utm_region.size);

    /* EPM may map anything, UTM may not map pgtables */
    if(!map_in_epm && (!map_in_utm || level != 1)){
      goto fatal_bail;
    }


    /* propagate the highest bit of the VA */
    if ( level == RISCV_PGLEVEL_TOP && i & RISCV_PGTABLE_HIGHEST_BIT )
      vpn = ((-1UL << RISCV_PGLEVEL_BITS) | (i & RISCV_PGLEVEL_MASK));
    else
      vpn = ((vaddr << RISCV_PGLEVEL_BITS) | (i & RISCV_PGLEVEL_MASK));

    uintptr_t va_start = vpn << RISCV_PGSHIFT;

    /* include the first virtual address of a contiguous range */
    if (level == 1 && !contiguous)
    {

      hash_extend(hash_ctx, &va_start, sizeof(uintptr_t));
      //printm("VA hashed: 0x%lx\n", va_start);
      contiguous = 1;
    }

    if (level == 1)
    {

      /*
       * This is where we enforce the at-most-one-mapping property.
       * To make our lives easier, we also require a 'linear' mapping
       * (for each of the user and runtime spaces independently).
       *
       * That is: Given V1->P1 and V2->P2:
       *
       * V1 < V2  ==> P1 < P2  (Only for within a given space)
       *
       * V1 != V2 ==> P1 != P2
       *
       * We also validate that all utm vaddrs -> utm paddrs
       */
      int in_runtime = ((phys_addr >= cargs->runtime_paddr) &&
                        (phys_addr < (cargs->user_paddr)));
      int in_user = ((phys_addr >= cargs->user_paddr) &&
                     (phys_addr < (cargs->free_paddr)));

      /* Validate U bit */
      if(in_user && !(*walk & PTE_U)){
        goto fatal_bail;
      }

      /* If the vaddr is in UTM, the paddr must be in UTM */
      if(va_start >= cargs->params.untrusted_ptr &&
         va_start < (cargs->params.untrusted_ptr + cargs->params.untrusted_size) &&
         !map_in_utm){
        goto fatal_bail;
      }

      /* Do linear mapping validation */
      if(in_runtime){
        if(phys_addr <= *runtime_max_seen){
          goto fatal_bail;
        }
        else{
          *runtime_max_seen = phys_addr;
        }
      }
      else if(in_user){
        if(phys_addr <= *user_max_seen){
          goto fatal_bail;
        }
        else{
          *user_max_seen = phys_addr;
        }
      }
      else if(map_in_utm){
        // we checked this above, its OK
      }
      else{
        //printm("BAD GENERIC MAP %x %x %x\n", in_runtime, in_user, map_in_utm);
        goto fatal_bail;
      }

      /* Page is valid, add it to the hash */

      /* if PTE is leaf, extend hash for the page */
      hash_extend_page(hash_ctx, (void*)phys_addr);



      //printm("PAGE hashed: 0x%lx (pa: 0x%lx)\n", vpn << RISCV_PGSHIFT, phys_addr);
    }
    else
    {
      /* otherwise, recurse on a lower level */
      contiguous = validate_and_hash_epm(hash_ctx,
                                         level - 1,
                                         (pte_t*) phys_addr,
                                         vpn,
                                         contiguous,
                                         cargs,
                                         runtime_max_seen,
                                         user_max_seen);
      if(contiguous == -1){
        printm("BAD MAP: %lx->%lx epm %x %lx uer %x %lx\n",
               va_start,phys_addr,
               //in_runtime,
               0,
               cargs->runtime_paddr,
               0,
               //in_user,
               cargs->user_paddr);
        goto fatal_bail;
      }
    }
  }

  return contiguous;

 fatal_bail:
  return -1;
}

enclave_ret_code validate_and_hash_enclave(struct enclave* enclave,
                                        struct keystone_sbi_create* cargs){

  hash_ctx hash_ctx;
  int ptlevel = RISCV_PGLEVEL_TOP;
  int i;

  hash_init(&hash_ctx);

  // hash the runtime parameters
  hash_extend(&hash_ctx, &enclave->params, sizeof(struct runtime_va_params_t));


  uintptr_t runtime_max_seen=0;
  uintptr_t user_max_seen=0;;

  // hash the epm contents including the virtual addresses
  int valid = validate_and_hash_epm(&hash_ctx,
                                    ptlevel,
                                    (pte_t*) (enclave->encl_satp << RISCV_PGSHIFT),
                                    0, 0, cargs, &runtime_max_seen, &user_max_seen);

  if(valid == -1){
    return ENCLAVE_ILLEGAL_PTE;
  }

  hash_finalize(enclave->hash, &hash_ctx);

  return ENCLAVE_SUCCESS;
}
