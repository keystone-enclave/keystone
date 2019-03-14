//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef __LOADER_H__
#define __LOADER_H__

#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_BITS 12
#define PAGE_SIZE (1UL<<PAGE_BITS)
#define ROUND_UP(n, b) (((((n) - 1ul) >> (b)) + 1ul) << (b))
#define ROUND_DOWN(n, b) (n & ~((2 << (b-1)) - 1))

class ELFFile 
{
  public:
    ELFFile(std::string filename);
    ~ELFFile();
    unsigned long setEntry(unsigned long _entry) { entry = _entry; return entry;}
    unsigned long getEntry() { return entry; }
    void* getPtr() { return ptr; }
    size_t getSize() { return size; }
    bool isValid();
  private:
    int filep;
    unsigned long entry;
    void* ptr;
    size_t size;
};

#endif /* loader */
