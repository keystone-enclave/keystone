//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

#include <assert.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>

#include "./common.h"
extern "C" {
#include "common/sha3.h"
}
#include "ElfFile.hpp"
#include "Error.hpp"
#include "KeystoneDevice.hpp"
#include "Memory.hpp"
#include "Params.hpp"

namespace Keystone {

typedef std::function<void(void*)> OcallFunc;

class Enclave {
 private:
  Params params;
  uintptr_t runtimeElfAddr;
  uintptr_t enclaveElfAddr;
  Memory* pMemory;
  KeystoneDevice* pDevice;
  void* shared_buffer;
  size_t shared_buffer_size;
  OcallFunc oFuncDispatch;
  bool mapUntrusted(size_t size);
  void copyMem(uintptr_t dest_offset, uintptr_t src, size_t size);
  void copyFile(uintptr_t filePtr, size_t fileSize);
  void allocUninitialized(ElfFile* elfFile);
  void loadElf(ElfFile* elfFile);

  bool initFiles(const char*, const char*);
  bool initDevice();
  bool prepareEnclaveMemory(size_t requiredPages, uintptr_t alternatePhysAddr);
  bool initMemory();

  typedef struct {
    char name[MSR_NAME_LEN];
    uintptr_t type;
    std::string filepath;
  } resource_info_t;
  std::vector<resource_info_t> identityResident;
  std::vector<resource_hash_t> identityAbsent;
  std::vector<resource_info_t> resident;
  std::vector<resource_hash_t> absent;
  void* enclave_base = 0;
  uintptr_t materializeResourceInfo(resource_ptr_t* residentResPtrs,
    ElfFile** allElfFiles, std::vector<resource_info_t> resInfos);

 public:
  Enclave();
  ~Enclave();
  static Error measure(char* hash, const char* eapppath, const char* runtimepath, const char* loaderpath);
  void* getSharedBuffer();
  size_t getSharedBufferSize();
  Memory* getMemory();
  uintptr_t getRuntimeElfAddr() { return runtimeElfAddr; }
  uintptr_t getEnclaveElfAddr() { return enclaveElfAddr; }
  Error registerOcallDispatch(OcallFunc func);
  Error init(const char* filepath, const char* runtime, const char* loaderpath, Params parameters);
  Error init(
      const char* eapppath, const char* runtimepath, const char* loaderpath, Params _params,
      uintptr_t alternatePhysAddr);
  Error destroy();
  Error run(uintptr_t* ret = nullptr);

  // TODO(Evgeny): switch to errors. Currently, 0 = success, 1 = error.
  uintptr_t addResidentResource(const char* name, uintptr_t type, const char* filepath, bool identity);
  uintptr_t addAbsentResource(const char* name, uintptr_t type, const char* hash, bool identity);
  uintptr_t finalize(uintptr_t alternatePhysAddr = 0);
};

uint64_t
calculate_required_pages(ElfFile** elfFiles, size_t numElfFiles);

}  // namespace Keystone
