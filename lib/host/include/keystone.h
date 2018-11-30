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

class Keystone;
typedef void (*OcallFunc)(void*);

typedef enum {
  KEYSTONE_ERROR=-1,
  KEYSTONE_SUCCESS,
  KEYSTONE_NOT_IMPLEMENTED,
} keystone_status_t;

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
};

#endif
