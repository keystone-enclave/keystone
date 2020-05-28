//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _KEYSTONE_H_
#define _KEYSTONE_H_

#include <stddef.h>
#include <cerrno>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <stdarg.h>
#include <assert.h>
#include "common.h"
#include "ELFFile.h"
#include "Params.h"
#include "sha3.h"
#include "Memory.h"
#include "KeystoneDevice.h"
#include "KeystoneError.h"

class Keystone;
typedef void (*OcallFunc)(void*);
typedef sha3_ctx_t hash_ctx_t;

class Keystone
{
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
  KeystoneError loadELF(ELFFile* file, uintptr_t* data_start);
  KeystoneError validate_and_hash_enclave(struct runtime_params_t args, struct keystone_hash_enclave* cargs);

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
  KeystoneError init(const char* filepath, const char* runtime, Params parameters);
  KeystoneError init(const char *eapppath, const char *runtimepath, Params _params, uintptr_t alternatePhysAddr);
  KeystoneError destroy();
  KeystoneError run();

};

unsigned long calculate_required_pages(
        unsigned long eapp_sz,
        unsigned long eapp_stack_sz,
        unsigned long rt_sz,
        unsigned long rt_stack_sz);

#endif
