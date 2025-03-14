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

namespace Keystone {

Enclave::Enclave() {
}

Enclave::Enclave(Params params) : params(params) {
}

Enclave::~Enclave() {
  assert(destroy() == Error::Success);
}

uint64_t
Enclave::calculateEpmPages(std::vector<ElfFile*> allElfFiles, size_t freeMemSize) {
  uint64_t pages = 0;

  for (ElfFile* elfFile : allElfFiles) {
    pages += PAGE_UP(elfFile->getFileSize()) / PAGE_SIZE;
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
  pages += 16;

  // FIXME: this will be deprecated with complete freemem support.
  // We just add freemem size for now.
  pages += PAGE_UP(freeMemSize) / PAGE_SIZE;
  return pages;
}

Error
Enclave::measureResidentArr(hash_ctx_t& hash_ctx, std::vector<resource_info_t> resident) {
  char sub_hash[MDSIZE];
  hash_ctx_t file_hash_ctx;
  for (resource_info_t& rInfo : resident) {
    hash_extend(&hash_ctx, &rInfo.name, sizeof(rInfo.name));
    hash_extend(&hash_ctx, &rInfo.type, sizeof(rInfo.type));

    // hash file
    ElfFile elfFile(rInfo.filepath);
    hash_init(&file_hash_ctx);
    hash_extend(&file_hash_ctx, elfFile.getPtr(), elfFile.getFileSize());
    hash_finalize(sub_hash, &file_hash_ctx);

    // hash(file hash)
    hash_extend(&hash_ctx, sub_hash, sizeof(sub_hash));
  }
  return Error::Success;
}

Error
Enclave::measureSelf(char* hash) {
  sortAllResources();

  hash_ctx_t hash_ctx;
  hash_init(&hash_ctx);

  // runtime vals
  runtime_val_t runtime_val = {.name = MSR_FREE_MEM, .val = params.getFreeMemSize()};
  hash_extend(&hash_ctx, &runtime_val, sizeof(runtime_val));
  runtime_val = {.name = MSR_UT_MEM, .val = params.getUntrustedSize()};
  hash_extend(&hash_ctx, &runtime_val, sizeof(runtime_val));

  measureResidentArr(hash_ctx, identityResident);
  for (resource_hash_t& rHash : identityAbsent) {
    hash_extend(&hash_ctx, &rHash, sizeof(rHash));
  }
  measureResidentArr(hash_ctx, resident);
  for (resource_hash_t& rHash : absent) {
    hash_extend(&hash_ctx, &rHash, sizeof(rHash));
  }

  hash_finalize(hash, &hash_ctx);
  
  return Error::Success;
}

Error
Enclave::measure(char* hash, const char* eapppath, const char* runtimepath, const char* loaderpath, Params params) {
  Enclave enclave;
  enclave.params = params;
  Error err = enclave.addStandard(eapppath, runtimepath, loaderpath);
  if (err != Error::Success) {
    return err;
  }
  enclave.measureSelf(hash);
  return Error::Success;
}

Error
Enclave::addResidentResource(const char* name, uintptr_t type, const char* filepath, bool identity) {
  if (strlen(name) >= MSR_NAME_LEN) {
    return Error::BadArgument;
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
  return Error::Success;
}

Error
Enclave::addAbsentResource(const char* name, uintptr_t type, const char* hash, bool identity) {
  if (strlen(name) >= MSR_NAME_LEN) {
    return Error::BadArgument;
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
  return Error::Success;
}

Error
Enclave::addStandard(const char* eapppath, const char* runtimepath, const char* loaderpath) {
  Error err = Error::Success;
  err = addResidentResource(MSR_START_FILENAME, 0, loaderpath, true);
  if (err != Error::Success) {
    ERROR("failed to add loader with path %s", loaderpath);
    return err;
  }
  err = addResidentResource(MSR_RUNTIME_FILENAME, 0, runtimepath, true);
  if (err != Error::Success) {
    ERROR("failed to add runtime with path %s", loaderpath);
    return err;
  }
  err = addResidentResource(MSR_EAPP_FILENAME, 0, eapppath, true);
  if (err != Error::Success) {
    ERROR("failed to add eapp with path %s", loaderpath);
    return err;
  }
  return err;
}

uintptr_t
Enclave::useEpm(uintptr_t src, uintptr_t size) {
  if (!size) {
    return pDevice.getEpmVirtAddr() + epmFreeOffset;
  }
  uintptr_t allocatedOffset = epmFreeOffset;
  epmFreeOffset = PAGE_UP(epmFreeOffset + size);
  assert(epmFreeOffset > allocatedOffset); // no overflow
  assert(epmFreeOffset <= pDevice.getEpmSize() - params.getFreeMemSize()); // free mem remaining
  uintptr_t dest_va = pDevice.getEpmVirtAddr() + allocatedOffset;
  if (src) {
    memcpy((void*) dest_va, (void*) src, size);
    uintptr_t end_va = dest_va + size;
    memset((void*) end_va, 0, pDevice.getEpmVirtAddr() + epmFreeOffset - end_va);
  }
  return dest_va;
}

Error
Enclave::materializeResourceInfo(resource_ptr_t residentResPtrs[], ElfFile* elfFiles[],
  std::vector<resource_info_t> resInfos) {
  for (uintptr_t i = 0; i < resInfos.size(); i++) {
    const resource_info_t& resInfo = resInfos[i];
    ElfFile* elfFile = elfFiles[i];
    resource_ptr_t* resPtr = residentResPtrs + i;
    strcpy(resPtr->name, resInfo.name);
    resPtr->type = resInfo.type;
    resPtr->offset = useEpm((uintptr_t) elfFile->getPtr(), elfFile->getFileSize()) - pDevice.getEpmVirtAddr();
    resPtr->size = (uintptr_t) elfFile->getFileSize();
  }
  return Error::Success;
}

bool
Enclave::resourceInfoCompare(const resource_info_t& a, const resource_info_t& b) {
  return strcmp(a.name, b.name) < 0;
}

bool
Enclave::resourceHashCompare(const resource_hash_t& a, const resource_hash_t& b) {
  return strcmp(a.name, b.name) < 0;
}

void
Enclave::sortAllResources() {
  // sort by filename
  std::sort(identityResident.begin(), identityResident.end(), resourceInfoCompare);
  std::sort(identityAbsent.begin(), identityAbsent.end(), resourceHashCompare);
  std::sort(resident.begin(), resident.end(), resourceInfoCompare);
  std::sort(absent.begin(), absent.end(), resourceHashCompare);
}

Error
Enclave::finalize() {
  sortAllResources();

  // confirm start executable is present
  bool startExecutablePresent = false;
  for (const resource_info_t& resInfo : identityResident) {
    if (strcmp(resInfo.name, MSR_START_FILENAME) == 0) {
      startExecutablePresent = true;
      break;
    }
  }
  if (!startExecutablePresent) {
    return Error::BadArgument;
  }
  
  Error err = Error::Success;
  pDevice = KeystoneDevice();
  epmFreeOffset = 0;

  err = pDevice.initDevice(params);
  if (err != Error::Success) {
    destroy();
    return err;
  }

  // allocate enclave memory
  for (const resource_info_t& resInfo : identityResident) {
    allElfFiles.push_back(new ElfFile(resInfo.filepath));
  }
  for (const resource_info_t& resInfo : resident) {
    allElfFiles.push_back(new ElfFile(resInfo.filepath));
  }
  uint64_t requiredPages = calculateEpmPages(allElfFiles, params.getFreeMemSize());
  err = pDevice.create(requiredPages);
  if (err != Error::Success) {
    destroy();
    return err;
  }
  err = pDevice.initUTM(params.getUntrustedSize());
  if (err != Error::Success) {
    ERROR("failed to init untrusted memory - ioctl() failed");
    destroy();
    return err;
  }

  uintptr_t ebase = useEpm(0, 0);
  enclave_bundle_header_t* ebundle_h = (enclave_bundle_header_t*) ebase;

  // space out the arrays
  ebundle_h->runtime_arr = (uintptr_t) sizeof(enclave_bundle_header_t);
  ebundle_h->id_res_arr = ebundle_h->runtime_arr
    + (uintptr_t) (sizeof(runtime_val_t) * 2);
  ebundle_h->id_abs_arr = ebundle_h->id_res_arr 
    + (uintptr_t) (sizeof(resource_ptr_t) * identityResident.size());
  ebundle_h->res_arr = ebundle_h->id_abs_arr 
    + (uintptr_t) (sizeof(resource_hash_t) * identityAbsent.size());
  ebundle_h->abs_arr = ebundle_h->res_arr 
    + (uintptr_t) (sizeof(resource_ptr_t) * resident.size());
  ebundle_h->pad_start = ebundle_h->abs_arr 
    + (uintptr_t) (sizeof(resource_hash_t) * absent.size());
  useEpm(0, ebundle_h->pad_start); // contiguous ebundle_h and arrays, then page padding

  // fill in the arrays and copy files
  runtime_val_t* runtime_arr = (runtime_val_t*) (ebase + ebundle_h->runtime_arr);
  runtime_arr[0] = {.name = MSR_FREE_MEM, .val = params.getFreeMemSize()};
  runtime_arr[1] = {.name = MSR_UT_MEM, .val = params.getUntrustedSize()};
  memcpy((void*) (ebase + ebundle_h->id_abs_arr), &identityAbsent[0], sizeof(resource_hash_t) * identityAbsent.size());
  memcpy((void*) (ebase + ebundle_h->abs_arr), &absent[0], sizeof(resource_hash_t) * absent.size());
  materializeResourceInfo((resource_ptr_t*) (ebase + ebundle_h->id_res_arr), &allElfFiles[0], identityResident);
  materializeResourceInfo((resource_ptr_t*) (ebase + ebundle_h->res_arr), &allElfFiles[identityResident.size()], resident);

  // finalize creating enclave
  err = pDevice.finalize(epmFreeOffset);
  if (err != Error::Success) {
    destroy();
    return err;
  }
  err = pDevice.mapUtm();
  if (err != Error::Success) {
    ERROR(
        "failed to finalize enclave - cannot map the untrusted buffer "
        "pointer \n");
    destroy();
    return err;
  }

  return Error::Success;
}

Error
Enclave::finalize(const char* eapppath, const char* runtimepath, const char* loaderpath, Params _params) {
  params = _params;

  Error err = addStandard(eapppath, runtimepath, loaderpath);
  if (err != Error::Success) {
    return err;
  }
  return finalize();
}

Error
Enclave::destroy() {
  for (ElfFile* elfFile : allElfFiles) {
    delete elfFile;
  }
  allElfFiles.clear();
  return pDevice.destroy();
}

Error
Enclave::run(uintptr_t* retval) {
  Error ret = pDevice.run(retval);
  while (ret == Error::EdgeCallHost || ret == Error::EnclaveInterrupted) {
    /* enclave is stopped in the middle. */
    if (ret == Error::EdgeCallHost && oFuncDispatch != NULL) {
      oFuncDispatch(getSharedBuffer());
    }
    ret = pDevice.resume(retval);
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
  assert(pDevice.getUtmVirtAddr());
  return (void*) pDevice.getUtmVirtAddr();
}

size_t
Enclave::getSharedBufferSize() {
  assert(pDevice.getUtmSize());
  return pDevice.getUtmSize();
}

Error
Enclave::registerOcallDispatch(OcallFunc func) {
  oFuncDispatch = func;
  return Error::Success;
}

}  // namespace Keystone
