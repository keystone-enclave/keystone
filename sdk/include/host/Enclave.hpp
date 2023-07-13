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
#include <cstdint>
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

using OcallFunc = std::function<void(void*)>;

class Enclave {
 private:
  Params params;
  ElfFile* runtimeFile;
  ElfFile* enclaveFile;
  Memory* pMemory;
  KeystoneDevice* pDevice;
  char hash[MDSIZE];
  hash_ctx_t hash_ctx;
  std::uintptr_t runtime_stk_sz;
  void* shared_buffer;
  std::size_t shared_buffer_size;
  OcallFunc oFuncDispatch;
  bool mapUntrusted(std::size_t size);
  bool allocPage(std::uintptr_t va, std::uintptr_t src, unsigned int mode);
  bool initStack(std::uintptr_t start, std::size_t size, bool is_rt);
  Error loadUntrusted();
  bool mapElf(ElfFile* file);
  Error loadElf(ElfFile* file);
  Error validate_and_hash_enclave(struct runtime_params_t args);

  bool initFiles(const char*, const char*);
  bool initDevice();
  bool prepareEnclave(std::uintptr_t alternatePhysAddr);
  bool initMemory();

 public:
  Enclave();
  ~Enclave();
  const char* getHash() const noexcept;
  void* getSharedBuffer() noexcept;
  std::size_t getSharedBufferSize() const noexcept;
  Error registerOcallDispatch(OcallFunc func);
  Error init(const char* filepath, const char* runtime, Params parameters);
  Error init(
      const char* eapppath, const char* runtimepath, Params _params,
      std::uintptr_t alternatePhysAddr);
  Error destroy();
  Error run(std::uintptr_t* ret = nullptr);
};

std::uint64_t
calculate_required_pages(
    std::uint64_t eapp_sz, std::uint64_t eapp_stack_sz, std::uint64_t rt_sz,
    std::uint64_t rt_stack_sz);

}  // namespace Keystone
