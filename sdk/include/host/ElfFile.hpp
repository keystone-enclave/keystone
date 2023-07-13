//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstdint>
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
  std::size_t getFileSize() const noexcept { return fileSize; }
  bool isValid() const noexcept;

  std::uintptr_t getMinVaddr() const noexcept { return minVaddr; }
  std::size_t getTotalMemorySize() const noexcept { return maxVaddr - minVaddr; }
  bool initialize(bool isRuntime);

  unsigned int getPageMode() const noexcept { return (isRuntime ? RT_FULL : USER_FULL); }

  /* libelf wrapper function */
  std::size_t getNumProgramHeaders(void);
  std::size_t getProgramHeaderType(std::size_t ph);
  std::size_t getProgramHeaderFileSize(std::size_t ph);
  std::size_t getProgramHeaderMemorySize(std::size_t ph);
  std::uintptr_t getProgramHeaderVaddr(std::size_t ph);
  std::uintptr_t getEntryPoint();
  void* getProgramSegment(std::size_t ph);

 private:
  int filep;

  /* virtual addresses */
  std::uintptr_t minVaddr;
  std::uintptr_t maxVaddr;

  void* ptr;
  std::size_t fileSize;

  /* is this runtime binary */
  bool isRuntime;

  /* libelf structure */
  elf_t elf;
};

}  // namespace Keystone
