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
  hash_ctx_t hash_ctx;
  hash_init(&hash_ctx);

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
Enclave::measure(char* hash, const char* eapppath, const char* runtimepath, const char* loaderpath) {
  Enclave enclave;
  Error err = Error::Success;
  err = enclave.addResidentResource("loader", 0, loaderpath, true);
  if (err != Error::Success) {
    ERROR("failed to add loader with path %s", loaderpath);
    return err;
  }
  err = enclave.addResidentResource("runtime", 0, runtimepath, true);
  if (err != Error::Success) {
    ERROR("failed to add runtime with path %s", loaderpath);
    return err;
  }
  err = enclave.addResidentResource("eapp", 0, eapppath, true);
  if (err != Error::Success) {
    ERROR("failed to add eapp with path %s", loaderpath);
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

Error
Enclave::finalize() {
  // TODO(Evgeny): ensure this is not called twice, no adds after, etc.
  // TODO(Evgeny): improve error messages
  // TODO(Evgeny): add comments to functions
  
  Error err = Error::Success;
  pDevice = KeystoneDevice();
  epmFreeOffset = 0;

  err = pDevice.initDevice(params);
  if (err != Error::Success) {
    destroy();
    return err;
  }

  // allocate enclave memory
  for (const resource_info_t& res_info : identityResident) {
    allElfFiles.push_back(new ElfFile(res_info.filepath));
  }
  for (const resource_info_t& res_info : resident) {
    allElfFiles.push_back(new ElfFile(res_info.filepath));
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
  ebundle_h->id_res_arr = ebundle_h->runtime_arr + 0; // TODO(Evgeny)
  ebundle_h->id_abs_arr = ebundle_h->id_res_arr 
    + (uintptr_t) (sizeof(resource_ptr_t) * identityResident.size());
  ebundle_h->res_arr = ebundle_h->id_abs_arr 
    + (uintptr_t) (sizeof(resource_hash_t) * identityAbsent.size());
  ebundle_h->abs_arr = ebundle_h->res_arr 
    + (uintptr_t) (sizeof(resource_ptr_t) * resident.size());
  ebundle_h->data = ebundle_h->abs_arr 
    + (uintptr_t) (sizeof(resource_hash_t) * absent.size());
  useEpm(0, ebundle_h->data); // contiguous ebundle_h and arrays, then page padding

  // fill in the arrays & data
  // TODO(Evgeny): runtime values
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
        "failed to finalize enclave - cannot obtain the untrusted buffer "
        "pointer \n");
    destroy();
    return err;
  }

  // TODO(Evgeny): validate that loader is present
  return Error::Success;
}

Error
Enclave::init(const char* eapppath, const char* runtimepath, const char* loaderpath, Params _params) {
  params = _params;

  Error err = Error::Success;
  err = addResidentResource("loader", 0, loaderpath, true);
  if (err != Error::Success) {
    ERROR("failed to add loader with path %s", loaderpath);
    return err;
  }
  err = addResidentResource("runtime", 0, runtimepath, true);
  if (err != Error::Success) {
    ERROR("failed to add runtime with path %s", loaderpath);
    return err;
  }
  err = addResidentResource("eapp", 0, eapppath, true);
  if (err != Error::Success) {
    ERROR("failed to add eapp with path %s", loaderpath);
    return err;
  }
  err = finalize();
  
  return err;
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
