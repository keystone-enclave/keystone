//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------

#include <stddef.h>
#include <cerrno>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <stdarg.h>
#include <assert.h>
#include "common.h"


class Memory;

class Memory
{
private:
  vaddr_t start_phys_addr;
  int keystone_fd;
  bool is_phys;
public:
  Memory();
  ~Memory();
  void init(int fd, vaddr_t phys_addr,  bool _is_phys);
  vaddr_t ReadMem(vaddr_t src, size_t size);
  void WriteMem(vaddr_t src, vaddr_t dst, size_t size);
  vaddr_t AllocMem(size_t size);
};


