//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef __ENCLAVE_CREATION_PARAMS_H__
#define __ENCLAVE_CREATION_PARAMS_H__

#define DEFAULT_RUNTIME_ENTRY   0xffffffffc0000000
#define DEFAULT_STACK_SIZE      1024*1024 // 1 MB
#define DEFAULT_UNTRUSTED_PTR   0xffffffffe0000000
#define DEFAULT_UNTRUSTED_SIZE  8192 // 8 KB

/* parameters for enclave creation */
struct enclave_params_t {
  uint64_t runtime_entry;
  uint64_t runtime_stack_size;
  uint64_t enclave_entry;
  uint64_t enclave_stack_size;
  uint64_t untrusted;
  uint64_t untrusted_size;
};

class Params 
{
  public:
    Params() { 
      params.runtime_stack_size = DEFAULT_STACK_SIZE;
      params.enclave_stack_size = DEFAULT_STACK_SIZE;
      params.runtime_entry = DEFAULT_RUNTIME_ENTRY;
      params.untrusted = DEFAULT_UNTRUSTED_PTR;
      params.untrusted_size = DEFAULT_UNTRUSTED_SIZE;
    }
    void setRuntimeEntry(uint64_t entry) { params.runtime_entry = entry; }
    void setRuntimeStack(uint64_t size) { params.runtime_stack_size = size; }
    void setEnclaveEntry(uint64_t entry) { params.enclave_entry = entry; }
    void setEnclaveStack(uint64_t size) { params.enclave_stack_size = size; }
    void setUntrustedMem(uint64_t ptr, uint64_t size) { params.untrusted = ptr; params.untrusted_size = size; }
    uint64_t getRuntimeEntry() { return params.runtime_entry; }
    uint64_t getRuntimeStack() { return params.runtime_stack_size; }
    uint64_t getEnclaveEntry() { return params.enclave_entry; }
    uint64_t getEnclaveStack() { return params.enclave_stack_size; }
    uint64_t getUntrustedMem() { return params.untrusted; }
    uint64_t getUntrustedSize() { return params.untrusted_size; }
    struct enclave_params_t getParams() { return params; }
  private:
    struct enclave_params_t params;
};

#endif
