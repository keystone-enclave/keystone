//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _KEYSTONE_H_
#define _KEYSTONE_H_

#include <stddef.h>
#include <cerrno>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <stdarg.h>
#include "elffile.h"
#include "params.h"


#define BOOST_STRINGIZE(X) BOOST_DO_STRINGIZE(X)
#define BOOST_DO_STRINGIZE(X) #X

#define KEYSTONE_DEV_PATH "/dev/keystone_enclave"

#define MSG(str) "[Keystone SDK] " __FILE__ ":" BOOST_STRINGIZE(__LINE__) " : " str
#define ERROR(str, ...) fprintf(stderr, MSG(str) "\n", ##__VA_ARGS__)
#define PERROR(str) perror(MSG(str))

typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;

class Keystone;
typedef void (*OcallFunc)(void*);

typedef enum {
  KEYSTONE_ERROR=-1,
  KEYSTONE_SUCCESS,
  KEYSTONE_NOT_IMPLEMENTED,
} keystone_status_t;


struct addr_packed {
    vaddr_t va;
    vaddr_t copied;
    unsigned int eid;
    unsigned int mode;
};

class Keystone
{
private:
  ELFFile* runtimeFile;
  ELFFile* enclaveFile;
  vaddr_t enclave_stk_start;
  vaddr_t enclave_stk_sz;
  vaddr_t runtime_stk_sz;
  vaddr_t untrusted_size;
  vaddr_t untrusted_start;
  int eid;
  int fd;
  void* shared_buffer;
  size_t shared_buffer_size;
  OcallFunc oFuncDispatch;
  keystone_status_t mapUntrusted(size_t size);
  keystone_status_t loadUntrusted(void);
  keystone_status_t loadRuntime(void);
  keystone_status_t loadApp(void);
public:
  Keystone();
  ~Keystone();
  void* getSharedBuffer();
  size_t getSharedBufferSize();
  keystone_status_t registerOcallDispatch(OcallFunc func);
  keystone_status_t init(const char* filepath, const char* runtime, Params parameters);
  keystone_status_t destroy();
  keystone_status_t run();

};

unsigned long calculate_required_pages(
        unsigned long eapp_sz,
        unsigned long eapp_stack_sz,
        unsigned long rt_sz,
        unsigned long rt_stack_sz);

#endif
