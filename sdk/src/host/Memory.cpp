//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "Memory.hpp"
#include <keystone_user.h>
#include <sys/stat.h>

namespace Keystone {

Memory::Memory() {
  epmFreeList   = 0;
  utmFreeList   = 0;
  startAddr     = 0;
}

void
Memory::startRuntimeMem() {
  runtimePhysAddr = getCurrentEPMAddress();
}

void
Memory::startEappMem() {
  eappPhysAddr = getCurrentEPMAddress();
}

void
Memory::startFreeMem() {
  freePhysAddr = getCurrentEPMAddress();
}

void 
Memory::incrementEPMFreeList() {
	epmFreeList += PAGE_SIZE;
}

uintptr_t
Memory::allocPages(size_t size) {
	uintptr_t addr = epmFreeList; 
	if (size % PAGE_SIZE > 0) {
    epmFreeList += (size / PAGE_SIZE + 1) * PAGE_SIZE;
  } else {
    epmFreeList += (size / PAGE_SIZE) * PAGE_SIZE;
  }
	return addr; 
}


/* This will walk the entire vaddr space in the enclave, validating
   linear at-most-once paddr mappings, and then hashing valid pages */
int
Memory::validateAndHashEpm(
    hash_ctx_t* hash_ctx, int level, pte* tb, uintptr_t vaddr, int contiguous,
    uintptr_t* runtime_max_seen, uintptr_t* user_max_seen) {
	return 0;
}

}  // namespace Keystone
