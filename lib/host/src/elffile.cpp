//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "elffile.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <cstdio>
#include "common.h"
#include "elffile.h"

static size_t fstatFileSize(int filep)
{
  int rc;
  struct stat stat_buf;
  rc = fstat(filep, &stat_buf);
  return (rc == 0 ? stat_buf.st_size : 0);
}

ELFFile::ELFFile(std::string filename)
{
  fileSize = 0;
  ptr = NULL;
  filep = open(filename.c_str(), O_RDONLY);

  if(filep < 0) {
    ERROR("file does not exist - %s",filename.c_str());
    return;
  }

  fileSize = fstatFileSize(filep);
  if(!fileSize) {
    ERROR("invalid file size - %s",filename.c_str());
  }

  ptr = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, filep, 0);

  if(!ptr) {
    ERROR("mmap failed for %s",filename.c_str());
  }

}

ELFFile::~ELFFile()
{
  close(filep);
  munmap(ptr, fileSize);
}

bool ELFFile::isValid()
{
  return (filep > 0 && fileSize > 0 && ptr != NULL);
}

bool ELFFile::initialize(bool _isRuntime)
{
  if(!isValid())
    return false;

  /* preparation for libelf */
  if (elf_newFile(ptr, fileSize, &elf)) {
    return false;
  }

  /* get bound vaddrs */
  elf_getMemoryBounds(&elf, VIRTUAL, &minVaddr, &maxVaddr);

  if(!IS_ALIGNED(minVaddr, PAGE_SIZE)) {
    return false;
  }

  maxVaddr = ROUND_UP(maxVaddr, PAGE_BITS);

  isRuntime = _isRuntime;

  return true;
}

/* Functions below are wrappers for libelf */
size_t ELFFile::getNumProgramHeaders(void)
{
  return elf_getNumProgramHeaders(&elf);
}

size_t ELFFile::getProgramHeaderType(size_t ph)
{
  return elf_getProgramHeaderType(&elf, ph);
}

size_t ELFFile::getProgramHeaderFileSize(size_t ph)
{
  return elf_getProgramHeaderFileSize(&elf, ph);
}

size_t ELFFile::getProgramHeaderMemorySize(size_t ph)
{
  return elf_getProgramHeaderMemorySize(&elf, ph);
}

vaddr_t ELFFile::getProgramHeaderVaddr(size_t ph)
{
  return elf_getProgramHeaderVaddr(&elf, ph);
}

vaddr_t ELFFile::getEntryPoint()
{
  return elf_getEntryPoint(&elf);
}

void* ELFFile::getProgramSegment(size_t ph)
{
  return elf_getProgramSegment(&elf, ph);
}

