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

struct mapped_meta {
    struct addr_packed *meta;
    int num_pages;
    unsigned int eid;
    unsigned int mode;
};

class Keystone
{
private:
  ELFFile* runtimeFile;
  ELFFile* enclaveFile;
  int eid;
  int fd;
  void* shared_buffer;
  size_t shared_buffer_size;
  OcallFunc oFuncDispatch;
  keystone_status_t mapUntrusted(size_t size);
public:
  Keystone();
  ~Keystone();
  void* getSharedBuffer();
  size_t getSharedBufferSize();
  keystone_status_t registerOcallDispatch(OcallFunc func);
  keystone_status_t init(const char* filepath, const char* runtime, Params parameters);
  keystone_status_t destroy();
  keystone_status_t run();
  keystone_status_t initRuntime(const char* filename);
  struct mapped_meta *keystone_rtld_init_rt_stk(vaddr_t stack_addr, unsigned long size, int *error);
  struct mapped_meta *keystone_rtld_init_app_stk(size_t app_stack_sz, unsigned long stack_offset, int *error);
  struct mapped_meta *keystone_app_load_elf_section_NOBITS(void *target_vaddr, size_t len, int *error);
  struct mapped_meta *keystone_app_load_elf_region(vaddr_t elf_usr_region, void *target_vaddr, size_t len, int *error);
  struct mapped_meta *rtld_vm_mmap(vaddr_t encl_addr, unsigned long size, vaddr_t rt_ptr, struct elf64_phdr *phdr, int *error);
  struct mapped_meta **keystone_rtld_init_app(vaddr_t elf_usr_ptr, int *num_pages, int *error);
//    struct mapped_meta **keystone_rtld_init_app();
  struct mapped_meta **keystone_rtld_init_runtime(vaddr_t rt_ptr, size_t rt_sz, unsigned long *rt_offset, int *num_rt_pages, int *error);

};

unsigned long calculate_required_pages(
        unsigned long eapp_sz,
        unsigned long eapp_stack_sz,
        unsigned long rt_sz,
        unsigned long rt_stack_sz);

#endif
