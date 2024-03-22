//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "Enclave.hpp"
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
extern "C" {
#include "common/sha3.h"
#include "shared/keystone_user.h"
}
#include "ElfFile.hpp"
#include "hash_util.hpp"

namespace Keystone {

Enclave::Enclave() {
}

Enclave::~Enclave() {
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
  /* FIXME Part 2: now that loader does loading, .bss sections also eat up
   * space. Eapp dev must make FREEMEM big enough to fit this!!
   * Possible fix -- re-add exact .bss calculations?
   */

  /* Add one page each for bss segments of runtime and eapp */ 
  // TODO: add space for stack?
  req_pages += 16;
  return req_pages;
}

bool
Enclave::prepareEnclaveMemory(size_t requiredPages, uintptr_t alternatePhysAddr) {
  // FIXME: this will be deprecated with complete freemem support.
  // We just add freemem size for now.
  uint64_t minPages;
  minPages = ROUND_UP(params.getFreeMemSize(), PAGE_BITS) / PAGE_SIZE; 
  minPages += requiredPages;

  /* Call Enclave Driver */
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

void
Enclave::copyFile(uintptr_t filePtr, size_t fileSize) {
	uintptr_t startOffset = pMemory->getCurrentOffset(); 
  size_t bytesRemaining = fileSize; 
	
	uintptr_t currOffset;
	while (bytesRemaining > 0) {
		currOffset = pMemory->getCurrentOffset(); 
    pMemory->incrementEPMFreeList();

		size_t bytesToWrite = (bytesRemaining > PAGE_SIZE) ? PAGE_SIZE : bytesRemaining;
		size_t bytesWritten = fileSize - bytesRemaining;

    // need 0 padding for hashes to be consistent,
    // and to keep code aligned to be able to map page-wise without copying.
    if (bytesToWrite < PAGE_SIZE) {
      char page[PAGE_SIZE];
      memset(page, 0, PAGE_SIZE);
      memcpy(page, (const void*) (filePtr + bytesWritten), (size_t)(bytesToWrite));
      pMemory->writeMem((uintptr_t) page, currOffset, PAGE_SIZE);
    } else {
		  pMemory->writeMem(filePtr + bytesWritten, currOffset, bytesToWrite);
    }
		bytesRemaining -= bytesToWrite;
	}
}

static void measureElfFile(hash_ctx_t* hash_ctx, ElfFile* file) {
  uintptr_t fptr = (uintptr_t) file->getPtr();
  uintptr_t fend = fptr + (uintptr_t) file->getFileSize();

  for (; fptr < fend; fptr += PAGE_SIZE) {
    if (fend - fptr < PAGE_SIZE) {
      char page[PAGE_SIZE];
      memset(page, 0, PAGE_SIZE);
      memcpy(page, (const void*) fptr, (size_t)(fend-fptr));
      hash_extend_page(hash_ctx, (void*) page);
    } else {
      hash_extend_page(hash_ctx, (void*) fptr);
    }
  }
}

Error
Enclave::measure(char* hash, const char* eapppath, const char* runtimepath, const char* loaderpath) {
  hash_ctx_t hash_ctx;
  hash_init(&hash_ctx);

  ElfFile* loader = new ElfFile(loaderpath);
  ElfFile* runtime = new ElfFile(runtimepath);
  ElfFile* eapp = new ElfFile(eapppath);

  uintptr_t sizes[3] = { PAGE_UP(loader->getFileSize()), PAGE_UP(runtime->getFileSize()),
                          PAGE_UP(eapp->getFileSize()) };
  hash_extend(&hash_ctx, (void*) sizes, sizeof(sizes));

  measureElfFile(&hash_ctx, loader);
  delete loader;
  measureElfFile(&hash_ctx, runtime);
  delete runtime;
  measureElfFile(&hash_ctx, eapp);
  delete eapp;

  hash_finalize(hash, &hash_ctx);

  return Error::Success;
}

// TODO(Evgeny): switch to errors. Currently, 0 = success, 1 = error.
uintptr_t
Enclave::addResidentResource(const char* name, uintptr_t type, const char* filepath, bool identity) {
  if (strlen(name) >= MSR_NAME_LEN) {
    return 1;
  }
  resource_info_t* resInfo = 0;
  if (identity) {
    identityResident.push_back({});
    resInfo = &identityResident.back();
  } else {
    resident.push_back({});
    resInfo = &resident.back();
  }
  strcpy(resInfo->name, name);
  resInfo->type = type;
  resInfo->filepath = std::string(filepath);
  return 0;
}

uintptr_t
Enclave::addAbsentResource(const char* name, uintptr_t type, const char* hash, bool identity) {
  if (strlen(name) >= MSR_NAME_LEN) {
    return 1;
  }
  resource_hash_t* resHash = 0;
  if (identity) {
    identityAbsent.push_back({});
    resHash = &identityAbsent.back();
  } else {
    absent.push_back({});
    resHash = &absent.back();
  }
  strcpy(resHash->name, name);
  resHash->type = type;
  memcpy(resHash->hash, hash, MDSIZE);
  return 0;
}

uintptr_t
Enclave::materializeResourceInfo(resource_ptr_t* residentResPtrs, ElfFile** elfFiles,
  std::vector<resource_info_t> resInfos) {
  for (uintptr_t i = 0; i < resInfos.size(); i++) {
    const resource_info_t& resInfo = resInfos[i];
    ElfFile* elfFile = elfFiles[i];
    resource_ptr_t* resPtr = residentResPtrs + i;
    strcpy(resPtr->name, resInfo.name);
    resPtr->type = resInfo.type;
    resPtr->offset = pMemory->getCurrentOffset();
    resPtr->size = (uintptr_t) elfFile->getFileSize();
    copyFile((uintptr_t) elfFile->getPtr(), elfFile->getFileSize());
  }
  return 0;
}

uintptr_t
Enclave::finalize(uintptr_t alternatePhysAddr) {
  // TODO(Evgeny): ensure this is not called twice, no adds after, etc.
  // TODO(Evgeny): how is alternatePhysAddr useful?
  
  pMemory = new PhysicalEnclaveMemory();
  pDevice = new KeystoneDevice();

  if (!pDevice->initDevice(params)) {
    destroy();
    return 1;
    // return Error::DeviceInitFailure;
  }

  // allocate enclave memory
  std::vector<ElfFile*> allElfFiles;
  for (const resource_info_t& res_info : identityResident) {
    allElfFiles.push_back(new ElfFile(res_info.filepath));
  }
  for (const resource_info_t& res_info : resident) {
    allElfFiles.push_back(new ElfFile(res_info.filepath));
  }
  size_t requiredPages = calculate_required_pages(&allElfFiles[0], allElfFiles.size());
  if (!prepareEnclaveMemory(requiredPages, alternatePhysAddr)) {
    destroy();
    return 1;
    // return Error::DeviceError;
  }
  if (!pMemory->allocUtm(params.getUntrustedSize())) {
    ERROR("failed to init untrusted memory - ioctl() failed");
    destroy();
    return 1;
    // return Error::DeviceError;
  }

  uintptr_t va_epm = pMemory->vaStartAddr;
  enclave_bundle_header_t* ebundle_h = (enclave_bundle_header_t*) va_epm;

  // space out the arrays
  ebundle_h->runtime_arr = (uintptr_t) sizeof(enclave_bundle_header_t);
  ebundle_h->id_res_arr = ebundle_h->runtime_arr + 0; // TODO(Evgeny)
  ebundle_h->id_abs_arr = ebundle_h->id_res_arr 
    + (uintptr_t) (sizeof(resource_ptr_t) * identityResident.size());
  ebundle_h->res_arr = ebundle_h->id_abs_arr 
    + (uintptr_t) (sizeof(resource_hash_t) * identityAbsent.size());
  ebundle_h->abs_arr = ebundle_h->res_arr 
    + (uintptr_t) (sizeof(resource_ptr_t) * resident.size());
  ebundle_h->data = ebundle_h->abs_arr 
    + (uintptr_t) (sizeof(resource_hash_t) * absent.size());
  pMemory->epmFreeList = ROUND_UP(ebundle_h->data, PAGE_BITS);

  // fill in the arrays & data
  // TODO(Evgeny): runtime values
  memcpy((void*) (va_epm + ebundle_h->id_abs_arr), &identityAbsent[0], sizeof(resource_hash_t) * identityAbsent.size());
  memcpy((void*) (va_epm + ebundle_h->abs_arr), &absent[0], sizeof(resource_hash_t) * absent.size());
  materializeResourceInfo((resource_ptr_t*) (va_epm + ebundle_h->id_res_arr), &allElfFiles[0], identityResident);
  // copyFile((uintptr_t) &tempIdentityResidentResourcePtrs[0], sizeof(resource_hash_t) * tempIdentityResidentResourcePtrs.size());
  // std::vector<resource_ptr_t> tempResidentResourcePtrs;
  // materializeResourceInfo(tempResidentResourcePtrs, &allElfFiles[identityResident.size()], resident);
  // copyFile((uintptr_t) &tempResidentResourcePtrs[0], sizeof(resource_hash_t) * tempResidentResourcePtrs.size());

  if (pDevice->finalize(pMemory->getCurrentOffset()) != Error::Success) {
    destroy();
    return 1;
    // return Error::DeviceError;
  }
  if (!mapUntrusted(params.getUntrustedSize())) {
    ERROR(
        "failed to finalize enclave - cannot obtain the untrusted buffer "
        "pointer \n");
    destroy();
    return 1;
    // return Error::DeviceMemoryMapError;
  }

  // TODO(Evgeny): validate that loader, runtime, and eapp are present
  return 0;
}

Error
Enclave::init(const char* eapppath, const char* runtimepath, const char* loaderpath, Params _params) {
  return this->init(eapppath, runtimepath, loaderpath, _params, (uintptr_t)0);
}

Error
Enclave::init(
    const char* eapppath, const char* runtimepath, const char* loaderpath, Params _params,
    uintptr_t alternatePhysAddr) {
  params = _params;

  // TODO(Evgeny): errors
  addResidentResource("loader", 0, loaderpath, true);
  addResidentResource("runtime", 0, runtimepath, true);
  addResidentResource("eapp", 0, eapppath, true);
  finalize(alternatePhysAddr);

  return Error::Success;
}

bool
Enclave::mapUntrusted(size_t size) {
  if (size == 0) {
    return true;
  }

  shared_buffer = pDevice->map(0, size);

  if (shared_buffer == NULL) {
    return false;
  }

  shared_buffer_size = size;

  return true;
}

Error
Enclave::destroy() {
  return pDevice->destroy();
}

Error
Enclave::run(uintptr_t* retval) {
  Error ret = pDevice->run(retval);
  while (ret == Error::EdgeCallHost || ret == Error::EnclaveInterrupted) {
    /* enclave is stopped in the middle. */
    if (ret == Error::EdgeCallHost && oFuncDispatch != NULL) {
      oFuncDispatch(getSharedBuffer());
    }
    ret = pDevice->resume(retval);
  }

  if (ret != Error::Success) {
    ERROR("failed to run enclave - ioctl() failed");
    destroy();
    return Error::DeviceError;
  }

  return Error::Success;
}

void*
Enclave::getSharedBuffer() {
  return shared_buffer;
}

size_t
Enclave::getSharedBufferSize() {
  return shared_buffer_size;
}

Memory*
Enclave::getMemory() {
  return pMemory;
}

Error
Enclave::registerOcallDispatch(OcallFunc func) {
  oFuncDispatch = func;
  return Error::Success;
}

}  // namespace Keystone
