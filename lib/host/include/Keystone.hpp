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
#include <iostream>
#include "./common.h"
extern "C" {
#include "./sha3.h"
}
#include "ELFFile.hpp"
#include "KeystoneDevice.hpp"
#include "KeystoneError.hpp"
#include "Memory.hpp"
#include "Params.hpp"

class Keystone;
typedef void (*OcallFunc)(void*);

class Keystone {
 private:
  Params params;
  ELFFile* runtimeFile;
  ELFFile* enclaveFile;
  Memory* pMemory;
  KeystoneDevice* pDevice;
  char hash[MDSIZE];
  hash_ctx_t hash_ctx;
  vaddr_t runtime_stk_sz;
  void* shared_buffer;
  size_t shared_buffer_size;
  OcallFunc oFuncDispatch;
  bool mapUntrusted(size_t size);
  bool allocPage(vaddr_t va, vaddr_t src, unsigned int mode);
  bool initStack(vaddr_t start, size_t size, bool is_rt);
  KeystoneError loadUntrusted();
  bool mapElf(ELFFile* file);
  KeystoneError loadElf(ELFFile* file);
  KeystoneError validate_and_hash_enclave(struct runtime_params_t args);

  bool initFiles(const char*, const char*);
  bool initDevice();
  bool prepareEnclave(uintptr_t alternatePhysAddr);
  bool initMemory();

 public:
  Keystone();
  ~Keystone();
  const char* getHash();
  void* getSharedBuffer();
  size_t getSharedBufferSize();
  KeystoneError registerOcallDispatch(OcallFunc func);
  KeystoneError init(
      const char* filepath, const char* runtime, Params parameters);
  KeystoneError init(
      const char* eapppath, const char* runtimepath, Params _params,
      uintptr_t alternatePhysAddr);
  KeystoneError destroy();
  KeystoneError run();
};

uint64_t
calculate_required_pages(
    uint64_t eapp_sz, uint64_t eapp_stack_sz, uint64_t rt_sz,
    uint64_t rt_stack_sz);
