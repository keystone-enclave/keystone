#include "fu540.h"
#include "encoding.h"
#include "sm-sbi.h"
#include "pmp.h"
#include "enclave.h"
#include <errno.h>
#include "page.h"
#include <string.h>

void platform_init_global(){

  waymask_init();

  /* Lock off part of the L2 for use as scratchpad */


}

void platform_init_enclave(struct platform_enclave_data* ped){
  ped->num_ways = WM_NUM_WAYS/2;
  ped->saved_mask = 0;
  ped->scratchpad_mask = 0;
}

void platform_create_enclave(struct platform_enclave_data* ped){
  /* TODO TMP way to try and get the scratchpad allocated */
  waymask_allocate_scratchpad(&ped->scratchpad_mask);

  /* Clear scratchpad for use */
  unsigned int core = read_csr(mhartid) + 1;
  waymask_apply_allocated_mask(ped->scratchpad_mask, core);

  /* High ways to low ways */
  /* TMP */
  uintptr_t addr;
  for(addr=L2_SCRATCH_START;
      addr<(L2_SCRATCH_START + (8 * L2_WAY_SIZE));
      addr+=sizeof(uintptr_t)){
    *(uintptr_t*)addr = 0;
  }
  /* TMP turn off access */
  _wm_assign_mask(0xFFFF,GET_CORE_DWAY(core));

  ped->scratch_rid = 0;
  /* TMP create pmp region */
  if(pmp_region_init_atomic(L2_SCRATCH_START, 8*L2_WAY_SIZE, PMP_PRI_ANY, &ped->scratch_rid, 0)){
    printm("FATAL CANNOT CREATE SCRATCH PMP\r\n");
  }

  printm("Created enclave, attempted to give it scratch\r\n");
}

void platform_destroy_enclave(struct platform_enclave_data* ped){
  if(ped->scratchpad_mask != 0){
    waymask_free_scratchpad(&ped->scratchpad_mask);
  }
  unsigned int core = read_csr(mhartid) + 1;
  _wm_assign_mask(0xFFFFFFFF,GET_CORE_DWAY(core));
}

void platform_switch_to_enclave(struct platform_enclave_data* ped){

  // Each hart gets special access to some
  //unsigned int core = read_csr(mhartid) + 1;

  /* Allocate ways, fresh every time we enter */
  /* size_t remaining = waymask_allocate_ways(ped->num_ways, */
  /*                                          core, */
  /*                                          &ped->saved_mask); */

  //printm("Chose ways: 0x%x, core 0x%x\r\n",ped->saved_mask, core);


  /* Assign the ways to all cores */
  //  waymask_apply_allocated_mask(ped->saved_mask, core);

  /* Clear out these ways MUST first apply mask to other masters */
  //waymask_clear_ways(ped->saved_mask, core);


  /* Setup PMP region for scratchpad */
  if(ped->scratch_rid != 0){
    pmp_set(ped->scratch_rid, PMP_ALL_PERM);
    printm("Switching to an enclave with scratchpad access\r\n");
  }


}

void platform_switch_from_enclave(struct platform_enclave_data* ped){
  /* Free all our ways */
  //  waymask_free_ways(ped->saved_mask);

  /* We don't need to clean them, see docs */

}
