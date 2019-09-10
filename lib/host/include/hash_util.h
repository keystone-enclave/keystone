//
// Created by Alex Thomas on 7/7/19.
//

void hash_init(hash_ctx_t* hash_ctx)
{
  sha3_init(hash_ctx, MDSIZE);
}

void hash_extend(hash_ctx_t* hash_ctx, const void* ptr, size_t len)
{
  sha3_update(hash_ctx, ptr, len);
}

void hash_extend_page(hash_ctx_t* hash_ctx, const void* ptr)
{
  sha3_update(hash_ctx, ptr, RISCV_PGSIZE);
}

void hash_finalize(void* md, hash_ctx_t* hash_ctx)
{
  sha3_final(md, hash_ctx);
}

void printHash(char *hash){
  for(int i = 0; i < MDSIZE; i+=sizeof(uintptr_t))
  {
    printf("%.16lx ", *((uintptr_t*) ((uintptr_t)hash + i)));
  }
  printf("\n");
}

/* This will walk the entire vaddr space in the enclave, validating
   linear at-most-once paddr mappings, and then hashing valid pages */
int validate_and_hash_epm(hash_ctx_t* hash_ctx, int level,
                          pte_t* tb, uintptr_t vaddr, int contiguous,
                          struct keystone_hash_enclave* cargs,
                          uintptr_t* runtime_max_seen,
                          uintptr_t* user_max_seen,
                          int fd)
{
  pte_t* walk;
  int i;

  /* iterate over PTEs */
  for (walk=tb, i=0; walk < tb + (RISCV_PGSIZE/sizeof(pte_t)); walk += 1,i++)
  {
    if (pte_val(*walk) == 0) {
      contiguous = 0;
      continue;
    }
    uintptr_t vpn;
    uintptr_t phys_addr = (pte_val(*walk) >> PTE_PPN_SHIFT) << RISCV_PGSHIFT;
    /* Check for blatently invalid mappings */
    int map_in_epm = (phys_addr >= cargs->epm_paddr &&
                      phys_addr < cargs->epm_paddr + cargs->epm_size);
    int map_in_utm = (phys_addr >= cargs->utm_paddr &&
                      phys_addr < cargs->utm_paddr + cargs->utm_size);

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
//      printf("user VA hashed: 0x%lx\n", va_start);
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
      if(in_user && !(pte_val(*walk) & PTE_U)){
        goto fatal_bail;
      }

      /* If the vaddr is in UTM, the paddr must be in UTM */
      if(va_start >= cargs->utm_paddr &&
         va_start < (cargs->utm_paddr + cargs->untrusted_size) &&
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
//        printf("BAD GENERIC MAP %x %x %x\n", in_runtime, in_user, map_in_utm);
        goto fatal_bail;
      }

      /* Page is valid, add it to the hash */

      /* if PTE is leaf, extend hash for the page */
      hash_extend_page(hash_ctx, (void*)phys_addr);
//      printf("user PAGE hashed: 0x%lx (pa: 0x%lx)\n", vpn << RISCV_PGSHIFT, phys_addr);
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
                                         user_max_seen,
                                         fd);
      if(contiguous == -1){
        printf("BAD MAP: %p->%p epm %u %p uer %u %p\n",
               (void *) va_start,
               (void *) phys_addr,
//                in_runtime,
                0,
               (void *) cargs->runtime_paddr,
//                in_user,
                0,
               (void *) cargs->user_paddr);
        goto fatal_bail;
      }
    }
  }

  return contiguous;

  fatal_bail:
  return -1;
}
