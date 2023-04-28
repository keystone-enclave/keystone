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

class LibraryEnclave {
 private:
  Params params;
  uintptr_t runtimeElfAddr;
  uintptr_t enclaveElfAddr;
  Memory* pMemory;
  KeystoneDevice* pDevice;
  char hash[MDSIZE];
  hash_ctx_t hash_ctx;
  uintptr_t copyFile(uintptr_t filePtr, size_t fileSize);
  Error validate_and_hash_enclave(struct runtime_params_t args);

  bool initFiles(const char*, const char*);
  bool initDevice();
  bool prepareEnclaveMemory(size_t requiredPages, uintptr_t alternatePhysAddr);
  bool initMemory();

 public:
  LibraryEnclave();
  ~LibraryEnclave();
  const char* getHash();
  Memory* getMemory();
  uintptr_t getRuntimeElfAddr() { return runtimeElfAddr; }
  uintptr_t getEnclaveElfAddr() { return enclaveElfAddr; }
  Error init( const char* dllpath, Params _params);
  Error destroy();
};

uint64_t
calculate_required_pages(ElfFile** elfFiles, size_t numElfFiles);

}  // namespace Keystone
