#include "sm.h"
#include "mtrap.h"
#include "pmp.h"
#include "atomic.h"

static int sm_init_done = 0;
static int sm_region_id, os_region_id;
static spinlock_t sm_init_lock = SPINLOCK_INIT;

typedef unsigned char byte;

extern byte sanctum_sm_signature[64];
extern byte sanctum_dev_public_key[32];
extern unsigned int sanctum_sm_size[1];

int smm_init()
{
  uint8_t perm = 0;
  int region = pmp_region_init(SMM_BASE, SMM_SIZE, perm, PMP_PRI_TOP);
  if(region < 0)
    return -1;

  return region;
}

int osm_init()
{
  uint8_t perm = PMP_W | PMP_X | PMP_R; 
  printm("osm_init\n");
  int region = pmp_region_init(0, -1UL, perm, PMP_PRI_BOTTOM); 
  printm("osm_init done\n");
  if(region < 0)
    return -1;

  return region;
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

void sm_init(void)
{
	// initialize SMM

  spinlock_lock(&sm_init_lock);
 
  if(!sm_init_done) {
    sm_region_id = smm_init();
    if(sm_region_id < 0)
      die("[SM] intolerable error - failed to initialize SM memory");

    os_region_id = osm_init();
    if(os_region_id < 0)
      die("[SM] intolerable error - failed to initialize OS memory");

    sm_init_done = 1;
  }
  
  pmp_set(sm_region_id);
  pmp_set(os_region_id);
  
  spinlock_unlock(&sm_init_lock);
  
  return;
  // for debug
  // sm_print_cert();
}
