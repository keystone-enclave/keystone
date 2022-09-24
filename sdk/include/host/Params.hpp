//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

#include <cstdio>

#if __riscv_xlen == 64
#define DEFAULT_FREEMEM_SIZE 1024 * 1024  // 1 MB
#define DEFAULT_UNTRUSTED_PTR 0xffffffff80000000
#define DEFAULT_STACK_SIZE 1024 * 16  // 16k
#define DEFAULT_STACK_START 0x0000000040000000
#elif __riscv_xlen == 32
#define DEFAULT_FREEMEM_SIZE 1024 * 512  // 512 KiB
#define DEFAULT_UNTRUSTED_PTR 0x80000000
#define DEFAULT_STACK_SIZE 1024 * 8  // 3 KiB
#define DEFAULT_STACK_START 0x40000000
#else                                     // for x86 tests
#define DEFAULT_FREEMEM_SIZE 1024 * 1024  // 1 MB
#define DEFAULT_UNTRUSTED_PTR 0xffffffff80000000
#define DEFAULT_STACK_SIZE 1024 * 16  // 16k
#define DEFAULT_STACK_START 0x0000000040000000
#endif

#define DEFAULT_UNTRUSTED_SIZE 8192  // 8 KB

/* parameters for enclave creation */
namespace Keystone {

class Params {
 public:
  Params() {
    simulated      = false;
    untrusted      = DEFAULT_UNTRUSTED_PTR;
    untrusted_size = DEFAULT_UNTRUSTED_SIZE;
    freemem_size   = DEFAULT_FREEMEM_SIZE;
  }

  void setSimulated(bool _simulated) { simulated = _simulated; }
  void setEnclaveEntry(uint64_t) {
    printf("WARN: setEnclaveEntry() is deprecated.\n");
  }
  void setUntrustedMem(uint64_t ptr, uint64_t size) {
    untrusted      = ptr;
    untrusted_size = size;
  }
  void setFreeMemSize(uint64_t size) { freemem_size = size; }
  bool isSimulated() { return simulated; }
  uintptr_t getUntrustedMem() { return untrusted; }
  uintptr_t getUntrustedSize() { return untrusted_size; }
  uintptr_t getUntrustedEnd() { return untrusted + untrusted_size; }
  uintptr_t getFreeMemSize() { return freemem_size; }

 private:
  bool simulated;
  uint64_t runtime_entry;
  uint64_t enclave_entry;
  uint64_t untrusted;
  uint64_t untrusted_size;
  uint64_t freemem_size;
};

}  // namespace Keystone
