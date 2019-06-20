#include "fu540.h"
#include "encoding.h"
#include "sm-sbi.h"
#include "pmp.h"
#include "enclave.h"
#include <errno.h>
#include "page.h"
#include <string.h>
void platform_init(struct platform_enclave_data_t* ped){
  ped->num_ways = WM_NUM_WAYS/2;
  ped->saved_mask = 0;
}

void platform_switch_to_enclave(struct platform_enclave_data_t* ped){

  // Each hart gets special access to some
  unsigned int core = read_csr(mhartid) + 1;

  /* Allocate ways, fresh every time we enter */
  size_t remaining = waymask_allocate_ways(ped->num_ways,
                                           core,
                                           &ped->saved_mask);

  //printm("Chose ways: 0x%x, core 0x%x\r\n",ped->saved_mask, core);


  /* Assign the ways to all cores */
  waymask_apply_allocated_mask(ped->saved_mask, core);

  /* Clear out these ways MUST first apply mask to other masters */
  /* uintptr_t cycles1,cycles2; */
  /* asm volatile ("rdcycle %0" : "=r" (cycles1)); */
  waymask_clear_ways(ped->saved_mask, core);
  /* asm volatile ("rdcycle %0" : "=r" (cycles2)); */
  /* printm("Wipe cost %lx\r\n",cycles2-cycles1); */

}

void platform_switch_from_enclave(struct platform_enclave_data_t* ped){
  /* Free all our ways */
  waymask_free_ways(ped->saved_mask);

  /* We don't need to clean them, see docs */

}
