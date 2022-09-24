//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "ElfFile.hpp"
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstdio>

namespace Keystone {

static size_t
fstatFileSize(int filep) {
  int rc;
  struct stat stat_buf;
  rc = fstat(filep, &stat_buf);
  return (rc == 0 ? stat_buf.st_size : 0);
}

ElfFile::ElfFile(std::string filename) {
  fileSize = 0;
  ptr      = NULL;
  filep    = open(filename.c_str(), O_RDONLY);

  if (filep < 0) {
    ERROR("file does not exist - %s", filename.c_str());
    return;
  }

  fileSize = fstatFileSize(filep);
  if (!fileSize) {
    ERROR("invalid file size - %s", filename.c_str());
  }

  ptr = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, filep, 0);

  if (!ptr) {
    ERROR("mmap failed for %s", filename.c_str());
  }
}

ElfFile::~ElfFile() {
  close(filep);
  munmap(ptr, fileSize);
}

bool
ElfFile::isValid() {
  return (filep > 0 && fileSize > 0 && ptr != NULL);
}

bool
ElfFile::initialize(bool _isRuntime) {
  if (!isValid()) return false;

  /* preparation for libelf */
  if (elf_newFile(ptr, fileSize, &elf)) {
    return false;
  }

  /* get bound vaddrs */
  elf_getMemoryBounds(&elf, VIRTUAL, &minVaddr, &maxVaddr);

  if (!IS_ALIGNED(minVaddr, PAGE_SIZE)) {
    return false;
  }

  maxVaddr = ROUND_UP(maxVaddr, PAGE_BITS);

  isRuntime = _isRuntime;

  return true;
}

/* Functions below are wrappers for libelf */
size_t
ElfFile::getNumProgramHeaders(void) {
  return elf_getNumProgramHeaders(&elf);
}

size_t
ElfFile::getProgramHeaderType(size_t ph) {
  return elf_getProgramHeaderType(&elf, ph);
}

size_t
ElfFile::getProgramHeaderFileSize(size_t ph) {
  return elf_getProgramHeaderFileSize(&elf, ph);
}

size_t
ElfFile::getProgramHeaderMemorySize(size_t ph) {
  return elf_getProgramHeaderMemorySize(&elf, ph);
}

uintptr_t
ElfFile::getProgramHeaderVaddr(size_t ph) {
  return elf_getProgramHeaderVaddr(&elf, ph);
}

uintptr_t
ElfFile::getEntryPoint() {
  return elf_getEntryPoint(&elf);
}

void*
ElfFile::getProgramSegment(size_t ph) {
  return elf_getProgramSegment(&elf, ph);
}
}  // namespace Keystone
