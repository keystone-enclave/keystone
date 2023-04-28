//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "LibraryEnclave.hpp"
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
extern "C" {
#include "./keystone_user.h"
#include "common/sha3.h"
}
#include "ElfFile.hpp"
#include "hash_util.hpp"

namespace Keystone {

LibraryEnclave::LibraryEnclave() {
}

LibraryEnclave::~LibraryEnclave() {
  destroy();
}

uint64_t
calculate_required_pages(ElfFile** elfFiles, size_t numElfFiles) {
  uint64_t req_pages = 0;

  for (int i = 0; i < numElfFiles; i++) {
    ElfFile* elfFile = elfFiles[i];
    req_pages += ceil(elfFile->getFileSize() / PAGE_SIZE);
  }
  /* FIXME: calculate the required number of pages for the page table.
   * We actually don't know how many page tables the enclave might need,
   * because the SDK never knows how its memory will be aligned.
   * Ideally, this should be managed by the driver.
   * For now, we naively allocate enough pages so that we can temporarily get
   * away from this problem.
   * 15 pages will be more than sufficient to cover several hundreds of
   * megabytes of enclave/runtime. */

  /* Add one page each for bss segments of runtime and eapp */ 
  // TODO: add space for stack?
  req_pages += 16;
  return req_pages;
}

bool
LibraryEnclave::prepareEnclaveMemory(size_t requiredPages, uintptr_t alternatePhysAddr) {
  // FIXME: this will be deprecated with complete freemem support.
  // We just add freemem size for now.
  uint64_t minPages;
  minPages = ROUND_UP(params.getFreeMemSize(), PAGE_BITS) / PAGE_SIZE; 
  minPages += requiredPages;

  if (params.isSimulated()) {
    pMemory->init(0, 0, minPages);
    return true;
  }

  /* Call LibraryEnclave Driver */
  if (pDevice->create(minPages) != Error::Success) {
    return false;
  }

  /* We switch out the phys addr as needed */
  uintptr_t physAddr;
  if (alternatePhysAddr) {
    physAddr = alternatePhysAddr;
  } else {
    physAddr = pDevice->getPhysAddr();
  }

  pMemory->init(pDevice, physAddr, minPages);
  return true;
}

uintptr_t
LibraryEnclave::copyFile(uintptr_t filePtr, size_t fileSize) {
	uintptr_t startOffset = pMemory->getCurrentOffset(); 
  size_t bytesRemaining = fileSize; 
	
	uintptr_t currOffset;
	while (bytesRemaining > 0) {
		currOffset = pMemory->getCurrentOffset(); 
    pMemory->incrementEPMFreeList();

		size_t bytesToWrite = (bytesRemaining > PAGE_SIZE) ? PAGE_SIZE : bytesRemaining;
		size_t bytesWritten = fileSize - bytesRemaining;

    if (bytesToWrite < PAGE_SIZE) {
      char page[PAGE_SIZE];
      memset(page, 0, PAGE_SIZE);
      memcpy(page, (const void*) filePtr + bytesWritten, (size_t)(bytesToWrite));
      pMemory->writeMem(filePtr + bytesWritten, currOffset, bytesToWrite);
    } else {
		  pMemory->writeMem(filePtr + bytesWritten, currOffset, bytesToWrite);
    }
		bytesRemaining -= bytesToWrite;
	}
	return startOffset;
}

Error
LibraryEnclave::validate_and_hash_enclave(struct runtime_params_t args) {
  return Error::Success;
}

const char*
LibraryEnclave::getHash() {
  return this->hash;
}

Error
LibraryEnclave::init(
    const char* dllpath, Params _params) {

  params = _params; // TODO: what params should this have

  if (params.isSimulated()) {
    pMemory = new SimulatedEnclaveMemory();
    pDevice = new MockKeystoneDevice();
  } else {
    pMemory = new PhysicalEnclaveMemory();
    pDevice = new KeystoneDevice();
  }
  
  ElfFile* dllFile = new ElfFile(dllpath);

  if (!pDevice->initDevice(params)) {
    destroy();
    return Error::DeviceInitFailure;
  }

  ElfFile* elfFiles[1] = {dllFile};
  size_t requiredPages = calculate_required_pages(elfFiles, 1);

  if (!prepareEnclaveMemory(requiredPages, 0)) {
    destroy();
    return Error::DeviceError;
  }

	copyFile((uintptr_t) dllFile->getPtr(), dllFile->getFileSize());

  if (pDevice->finalizeLibraryEnclave(basename(dllpath)) != Error::Success) {
    destroy();
    return Error::DeviceError;
  }

  printf("Finished initializing library enclave\n");
}

Error
LibraryEnclave::destroy() {
  return pDevice->destroyLibraryEnclave();
}

Memory*
LibraryEnclave::getMemory() {
  return pMemory;
}

}  // namespace Keystone
