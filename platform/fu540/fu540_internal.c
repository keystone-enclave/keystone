#include "fu540.h"
#include "encoding.h"
#include "sm-sbi.h"
#include "pmp.h"
#include "enclave.h"
#include <errno.h>
#include "page.h"
#include <string.h>

void scratch_init(){
  if(scratchpad_allocated_ways != 0){
    return;
  }

  /* TODO TMP way to try and get the scratchpad allocated */
  waymask_allocate_scratchpad(&scratchpad_allocated_ways);



  /* Clear scratchpad for use */
  unsigned int core = read_csr(mhartid);
  waymask_apply_allocated_mask(scratchpad_allocated_ways, core);

  waymask_t invert_mask = WM_FLIP_MASK(scratchpad_allocated_ways);
  _wm_assign_mask(invert_mask, core*2+1);

  /* This section is quite delicate, and may need to be re-written in
     assembly. Fundamentally, we are going to create a scratchpad
     region in the L2 based on the given mask (assuming that the mask
     is contiguous bits.
  */

  /* Choose a start/stop physical address to use for the
     scratchpad. As long as we choose contiguous addresses in the L2
     Zero Device that total the size of the allocated ways, they don't
     really matter */
  uintptr_t scratch_start = L2_SCRATCH_START;
  uintptr_t scratch_stop = L2_SCRATCH_START + (8 *  L2_WAY_SIZE);
  waymask_t tmp_mask;
  uintptr_t addr;

  addr = scratch_start;
  /* We will be directly setting the master d$ mask to avoid any cache
     pollution issues */
  waymask_t* master_mask = WM_REG_ADDR(core*2);
  /* Go through the mask one way at a time to control the allocations */
  for(tmp_mask=0x80;
      tmp_mask <= scratchpad_allocated_ways;
      tmp_mask = tmp_mask << 1){
    uintptr_t way_end  = addr + L2_WAY_SIZE;
    /* Assign a temporary mask of 1 way to the d$ */
    *master_mask = tmp_mask;
    /* Write a known value to every L2_LINE_SIZE offset */
    for(;
        addr < way_end;
        addr+= L2_LINE_SIZE){
      *(uintptr_t*)addr = 64;
    }
    /* Disable as soon as possible */
    *master_mask = invert_mask;
  }

  /* At this point, no master has waymasks for the scratchpad ways,
     and all scratchpad addresses have L2 lines */

  /* We try and check it now, any error SHOULD be immediately detectable. */
  for(addr = scratch_start; addr < scratch_stop; addr += L2_LINE_SIZE){
    if(*(uintptr_t*)addr != 64){
      printm("FATAL: Found a bad line %x\r\n", addr);
      /* TODO Not fatal! */
    }
  }

}

void platform_init_global_once(){

  waymask_init();
  scratchpad_allocated_ways = 0;

  /* PMP Lock the entire L2 controller */
  if(pmp_region_init_atomic(CACHE_CONTROLLER_ADDR_START,
                            CACHE_CONTROLLER_ADDR_END - CACHE_CONTROLLER_ADDR_START,
                            PMP_PRI_ANY, &l2_controller_rid, 1)){
    printm("FATAL CANNOT CREATE PMP FOR CONTROLLER\r\n");
    //TODO: this isn't currently fatal!
  }
  /* Create PMP region for scratchpad */
  if(pmp_region_init_atomic(L2_SCRATCH_START,
                            L2_SCRATCH_STOP - L2_SCRATCH_START,
                            PMP_PRI_ANY, &scratch_rid, 1)){
    printm("FATAL CANNOT CREATE SCRATCH PMP\r\n");
    //TODO: this isn't currently fatal!
  }
}


void platform_init_global(){
  pmp_set(l2_controller_rid, PMP_NO_PERM);
  pmp_set(scratch_rid, PMP_NO_PERM);

}

void platform_init_enclave(struct enclave* enclave){
  enclave->ped.num_ways = 0; // DISABLE waymasking
  //ped->num_ways = WM_NUM_WAYS/2;
  enclave->ped.saved_mask = 0;
}

void platform_create_enclave(struct enclave* enclave){
  scratch_init();

  enclave->ped.use_scratch = 1;
  //
}

void platform_destroy_enclave(struct enclave* enclave){
  enclave->ped.use_scratch = 0;

  /* TODO Must clean out the scratchpad if it was in use! */

}

void platform_switch_to_enclave(struct enclave* enclave){

  if(enclave->ped.num_ways > 0){
    // Each hart gets special access to some
    unsigned int core = read_csr(mhartid);

    //Allocate ways, fresh every time we enter
    size_t remaining = waymask_allocate_ways(enclave->ped.num_ways,
                                             core,
                                             &enclave->ped.saved_mask);

    //printm("Chose ways: 0x%x, core 0x%x\r\n",enclave->ped.saved_mask, core);
    /* Assign the ways to all cores */
    waymask_apply_allocated_mask(enclave->ped.saved_mask, core);

    /* Clear out these ways MUST first apply mask to other masters */
    waymask_clear_ways(enclave->ped.saved_mask, core);
  }

  /* Setup PMP region for scratchpad */
  if(enclave->ped.use_scratch != 0){
    pmp_set(scratch_rid, PMP_ALL_PERM);
    //printm("Switching to an enclave with scratchpad access\r\n");
  }
}

void platform_switch_from_enclave(struct enclave* enclave){
  if(enclave->ped.num_ways > 0){
    /* Free all our ways */
    waymask_free_ways(enclave->ped.saved_mask);
    /* We don't need to clean them, see docs */
  }
  if(enclave->ped.use_scratch != 0){
    pmp_set(scratch_rid, PMP_NO_PERM);
  }

}
