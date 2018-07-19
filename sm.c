#include "sm.h"
#include "mtrap.h"
#include "pmp.h"

typedef unsigned char byte;

extern byte sanctum_sm_signature[64];
extern byte sanctum_dev_public_key[32];

extern unsigned int sanctum_sm_size[1];

int smm_init(uintptr_t start, uint64_t size, uint8_t perm)
{
  int region = pmp_region_init(start, size, perm);
  if(region < 0)
  {
    printm("sm: failed to initialize a PMP region\n");
    return -1;
  }
  printm("sm: SMM PMP region index = %d\n", region);

  int reg = pmp_set(region);
  if(reg < 0)
  {
    printm("sm: failed to set PMP\n");
    pmp_region_debug_print(region);
    return -1;
  }

  return 0;
}

void sm_print_cert()
{
	int i;

	printm("Booting from Security Monitor\n");
	printm("Size: %d\n", sanctum_sm_size[0]);

	printm("============ PUBKEY =============\n");
	for(i=0; i<8; i+=1)
	{
		printm("%x",*((int*)sanctum_dev_public_key+i));
		if(i%4==3) printm("\n");
	}	
	printm("=================================\n");
	
	printm("=========== SIGNATURE ===========\n");
	for(i=0; i<16; i+=1)
	{
		printm("%x",*((int*)sanctum_sm_signature+i));
		if(i%4==3) printm("\n");
	}
	printm("=================================\n");
}

void set_tvm()
{
  uintptr_t mstatus = read_csr(mstatus);
  mstatus = mstatus | MSTATUS_TVM;
  write_csr(mstatus, mstatus);
}

void sm_init(void)
{
	// initialize SMM
	smm_init(SMM_BASE, SMM_SIZE, 0);

  set_tvm();
	sm_print_cert();
}
