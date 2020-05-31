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

class ELFFile {
 public:
  explicit ELFFile(std::string filename);
  ~ELFFile();
  size_t getFileSize() { return fileSize; }
  bool isValid();

  vaddr_t getMinVaddr() { return minVaddr; }
  size_t getTotalMemorySize() { return maxVaddr - minVaddr; }
  bool initialize(bool isRuntime);

  unsigned int getPageMode() { return (isRuntime ? RT_FULL : USER_FULL); }

  /* libelf wrapper function */
  size_t getNumProgramHeaders(void);
  size_t getProgramHeaderType(size_t ph);
  size_t getProgramHeaderFileSize(size_t ph);
  size_t getProgramHeaderMemorySize(size_t ph);
  vaddr_t getProgramHeaderVaddr(size_t ph);
  vaddr_t getEntryPoint();
  void* getProgramSegment(size_t ph);

 private:
  int filep;

  /* virtual addresses */
  vaddr_t minVaddr;
  vaddr_t maxVaddr;

  void* ptr;
  size_t fileSize;

  /* is this runtime binary */
  bool isRuntime;

  /* libelf structure */
  elf_t elf;
};
