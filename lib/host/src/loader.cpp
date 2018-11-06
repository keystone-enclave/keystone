#include "loader.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <cstdio>
#include "keystone.h"

ELFFile::ELFFile(std::string filename)
{
  int rc;
  struct stat stat_buf;

  size = 0;
  ptr = NULL;
  filep = open(filename.c_str(), O_RDONLY);
  if(filep < 0)
  {
    ERROR("file does not exist - %s",filename.c_str());
    return;
  }
  /* getting file size */
  rc = fstat(filep, &stat_buf);
  size = (rc == 0 ? stat_buf.st_size : 0);
  if(size == 0)
  {
    ERROR("invalid file size - %s",filename.c_str());
  }
  ptr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, filep, 0);
  if(!ptr)
  {
    ERROR("mmap failed for %s",filename.c_str());
  }
}

ELFFile::~ELFFile()
{
  close(filep);
  munmap(ptr, size);
}

bool ELFFile::isValid()
{
  return (filep > 0 && size > 0 && ptr != NULL);
}
