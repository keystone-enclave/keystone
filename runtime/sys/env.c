#include "sys/env.h"
#include "uaccess.h"
#include "util/rt_util.h"
#include "util/string.h"
/******
 *
 * This is for setting up a minimal ENV/AUX etc to let libc init
 * succeed inside an enclave.
 *
 * STACK LAYOUT
 *
 * 0xFFFFFFFF
 * AT_RANDOM bytes (16 bytes)
 *    [...]
 * auxv[1] VALUE
 * auxv[0] INDEX (AT_#)
 * envp[0] = NULL (no env)
 * argv = NULL
 * argc = 0
 * ----SP----
 *    [...]
 * 0x000000000
 *******/

// How many AUX things are we actually defining? Add one for terminator
#define AUXV_COUNT 13

// Size in number-of-words (argc, argv, null_env, auxv, randombytes
#define SIZE_OF_SETUP (1+1+1+(2*AUXV_COUNT) + 2)

// We return the new sp
void* setup_start(void* _sp, ELF(Ehdr) *hdr) {
  // Staging for eventual stack data


  void** sp = (void**)_sp;

#ifdef USE_ENV_SETUP
  int h;
  ELF(Phdr) *phdr;
  void* staging[SIZE_OF_SETUP];

  sp = sp - SIZE_OF_SETUP;

  //setup argc
  staging[0] = (void*)0;

  //argv
  staging[1] = (void*)0; // NULL
  //TODO We need to support /at least/ argv[0]

  //envp[0] (terminate)
  staging[2] = (void*)0; // NULL

  //AUX vectors
  unsigned long i = 0;
  void** at_random_ptr = 0;
  unsigned long* auxv = (unsigned long*)&(staging[3]);

  auxv[i++] = AT_HWCAP;
  auxv[i++] = 0x112d; // hardcoded from sample on non-enclave run
  auxv[i++] = AT_SYSINFO;
  auxv[i++] = 0x0;
  auxv[i++] = AT_PAGESZ;
  auxv[i++] = 0x1000;
  auxv[i++] = AT_EXECFN;
  auxv[i++] = 0; // NULL
  auxv[i++] = AT_SECURE;
  auxv[i++] = 0;
  auxv[i++] = AT_RANDOM;
  at_random_ptr = (void**)&(auxv[i]);
  auxv[i++] = 0; //"The address of sixteen bytes containing a random value."
  // We'll do that at the end

  // TODO: What do we want to do about UIDs and GIDs?
  auxv[i++] = AT_GID;
  auxv[i++] = 1;
  auxv[i++] = AT_EGID;
  auxv[i++] = 1;
  auxv[i++] = AT_UID;
  auxv[i++] = 1;
  auxv[i++] = AT_EUID;
  auxv[i++] = 1;

  //find virtual base
  phdr = (void *) hdr + hdr->e_phoff;
  for(h = 0; h < hdr->e_phnum; h++) {
    if(phdr[h].p_type == PT_LOAD && phdr[h].p_offset == 0) {
      auxv[i++] = AT_PHDR;
      auxv[i++] = phdr[h].p_vaddr + hdr->e_phoff;
      auxv[i++] = AT_PHNUM;
      auxv[i++] = hdr->e_phnum;
      break;
    }
  }

  auxv[i++] = AT_NULL;
  auxv[i++] = 0;
  // should be that i == AUXV_COUNT*2

  // Ask for random values
  size_t ret = rt_util_getrandom((void*)&auxv[i], 16);
  if( ret != 16 )  rt_util_misc_fatal();
  //Assign the ptr
  *at_random_ptr = &sp[i+3];
  i+=2; // fixup i

  // Copy staging to userstack
  copy_to_user(sp, staging, SIZE_OF_SETUP*sizeof(void*));

#endif /* USE_ENV_SETUP */

  // Fully setup, tell them to use this SP instead of the given one
  return sp;

}
