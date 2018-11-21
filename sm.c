#include <string.h>
#include "sm.h"
#include "mtrap.h"
#include "pmp.h"
#include "atomic.h"
#include "crypto.h"

static int sm_init_done = 0;
static int sm_region_id = 0, os_region_id = 0;
static spinlock_t sm_init_lock = SPINLOCK_INIT;

/* from Sanctum BootROM */
extern byte sanctum_sm_hash[MDSIZE];
extern byte sanctum_sm_signature[SIGNATURE_SIZE];
extern byte sanctum_sm_secret_key[PRIVATE_KEY_SIZE];
extern byte sanctum_sm_public_key[PUBLIC_KEY_SIZE];
extern byte sanctum_dev_public_key[PUBLIC_KEY_SIZE];

byte sm_hash[MDSIZE] = { 0, };
byte sm_signature[SIGNATURE_SIZE] = { 0, };
byte sm_public_key[PUBLIC_KEY_SIZE] = { 0, };
byte sm_private_key[PRIVATE_KEY_SIZE] = { 0, };
byte dev_public_key[PUBLIC_KEY_SIZE] = { 0, };

int osm_pmp_set(uint8_t perm)
{
  /* in case of OSM, PMP cfg is exactly the opposite.*/
  return pmp_set(os_region_id, perm);
}

int smm_init()
{
  int region = -1;
  int ret = pmp_region_init_atomic(SMM_BASE, SMM_SIZE, PMP_PRI_TOP, &region, 0);
  if(ret)
    return -1;

  return region;
}

int osm_init()
{
  int region = -1;
  int ret = pmp_region_init_atomic(0, -1UL, PMP_PRI_BOTTOM, &region, 1); 
  if(ret)
    return -1;

  return region;
}

void sm_sign(void* signature, const void* data, size_t len)
{
  sign(signature, data, len, sm_public_key, sm_private_key); 
}

void sm_copy_key()
{
  memcpy(sm_hash, sanctum_sm_hash, MDSIZE);
  memcpy(sm_signature, sanctum_sm_signature, SIGNATURE_SIZE);
  memcpy(sm_public_key, sanctum_sm_public_key, PUBLIC_KEY_SIZE);
  memcpy(sm_private_key, sanctum_sm_secret_key, PRIVATE_KEY_SIZE);
  memcpy(dev_public_key, sanctum_dev_public_key, PUBLIC_KEY_SIZE);
}

/*
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
*/

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
  
  pmp_set(sm_region_id, PMP_NO_PERM);
  pmp_set(os_region_id, PMP_ALL_PERM);

  // Copy the keypair from the root of trust
  sm_copy_key();
  spinlock_unlock(&sm_init_lock);
  
  return;
  // for debug
  // sm_print_cert();
}
