//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "./common.h"
#include "./keystone_user.h"

extern "C" {
#include "./elf.h"
}

namespace Keystone {

class ElfFile {
 public:
  explicit ElfFile(std::string filename);
  ~ElfFile();
  size_t getFileSize() { return fileSize; }
  bool isValid();

  uintptr_t getMinVaddr() { return minVaddr; }
  size_t getTotalMemorySize() { return maxVaddr - minVaddr; }
  bool initialize(bool isRuntime);

  unsigned int getPageMode() { return (isRuntime ? RT_FULL : USER_FULL); }

  /* libelf wrapper function */
  size_t getNumProgramHeaders(void);
  size_t getProgramHeaderType(size_t ph);
  size_t getProgramHeaderFileSize(size_t ph);
  size_t getProgramHeaderMemorySize(size_t ph);
  uintptr_t getProgramHeaderVaddr(size_t ph);
  uintptr_t getEntryPoint();
  void* getProgramSegment(size_t ph);

 private:
  int filep;

  /* virtual addresses */
  uintptr_t minVaddr;
  uintptr_t maxVaddr;

  void* ptr;
  size_t fileSize;

  /* is this runtime binary */
  bool isRuntime;

  /* libelf structure */
  elf_t elf;
};

}  // namespace Keystone
