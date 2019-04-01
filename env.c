#include "elf.h"
#include "tmplib/uaccess.h"

/******
 *
 * This is for setting up a minimal ENV/AUX etc to let libc init
 * succeed inside an enclave.
 *
 * STACK LAYOUT
 *
 * 0xFFFFFFFF
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
#define AUXV_COUNT 11


// Size in number-of-words (argc, argv, null_env, auxv)
#define SIZE_OF_SETUP (1+1+1+(2*AUXV_COUNT))

// We return the new sp
void* setup_start(void* _sp){
  // Staging for eventual stack data

  void* staging[SIZE_OF_SETUP];

  void** sp = (void**)_sp;
  sp = sp - SIZE_OF_SETUP;

  //setup argc
  staging[0] = 0;

  //argv
  staging[1] = (void*)0; // NULL

  //envp[0] (terminate)
  staging[2] = (void*)0; // NULL

  //AUX vectors
  unsigned long i = 0;
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
  auxv[i++] = 0; // TODO Need random value here for stack canaries

  // TODO: What do we want to do about UIDs and GIDs?
  auxv[i++] = AT_GID;
  auxv[i++] = 1;
  auxv[i++] = AT_EGID;
  auxv[i++] = 1;
  auxv[i++] = AT_UID;
  auxv[i++] = 1;
  auxv[i++] = AT_EUID;
  auxv[i++] = 1;
  auxv[i++] = AT_NULL;
  auxv[i++] = 0;

  // should be that i == AUXV_COUNT*2

  // We don't do PHDR or PHNUM etc right now, TLS might need it though

  // Copy staging to userstack
  copy_to_user(sp, staging, SIZE_OF_SETUP*sizeof(void*));

  // Fully setup, tell them to use this SP instead of the given one
  return sp;

}
