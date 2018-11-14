#ifndef __LOADER_H__
#define __LOADER_H__

#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

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
