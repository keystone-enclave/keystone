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
#include "Params.hpp"
#include "hash_util.hpp"

namespace Keystone {

typedef std::function<void(void*)> OcallFunc;

class Enclave {
 private:
  Params params;
  KeystoneDevice pDevice;
  OcallFunc oFuncDispatch;

  // track added resources
  typedef struct {
    char name[MSR_NAME_LEN];
    uintptr_t type;
    std::string filepath;
  } resource_info_t;
  std::vector<resource_info_t> identityResident;
  std::vector<resource_hash_t> identityAbsent;
  std::vector<resource_info_t> resident;
  std::vector<resource_hash_t> absent;
  std::vector<ElfFile*> allElfFiles;

  static uint64_t calculateEpmPages(std::vector<ElfFile*> allElfFiles, size_t freeMemSize);
  // linearly advances as we write to epm
  uintptr_t epmFreeOffset;
  /* Returns va to write to free epm after marking size bytes rounded up unfree.
     Copies and 0-pads from src if given. */
  uintptr_t useEpm(uintptr_t src, uintptr_t size);
  /* Iterates resident resources registered with the class and puts the files 
     and their pointers into the enclave bundle in epm. */
  Error materializeResourceInfo(resource_ptr_t residentResPtrs[],
    ElfFile* allElfFiles[], std::vector<resource_info_t> resInfos);
  static Error measureResidentArr(hash_ctx_t& hash_ctx, std::vector<resource_info_t> resident);
  static bool resourceInfoCompare(const resource_info_t& a, const resource_info_t& b);
  static bool resourceHashCompare(const resource_hash_t& a, const resource_hash_t& b);
  void sortAllResources();

 public:
  Enclave();
  Enclave(Params params);
  ~Enclave();
  Error measureSelf(char* hash);
  static Error measure(char* hash, const char* eapppath, const char* runtimepath, const char* loaderpath, Params params);
  // shared buffer is utm
  void* getSharedBuffer();
  size_t getSharedBufferSize();
  Error registerOcallDispatch(OcallFunc func);
  Error destroy();
  Error run(uintptr_t* ret = nullptr);

  Error addResidentResource(const char* name, uintptr_t type, const char* filepath, bool identity);
  Error addAbsentResource(const char* name, uintptr_t type, const char* hash, bool identity);
  Error addStandard(const char* eapppath, const char* runtimepath, const char* loaderpath);
  // Call after adding all needed resources to fully create the enclave.
  Error finalize();
  Error finalize(const char* filepath, const char* runtime, const char* loaderpath, Params _params);
};

}  // namespace Keystone
