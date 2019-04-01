#include "syscall.h"
#include "mm.h"

uintptr_t syscall_mmap(void *addr, size_t length, int prot, int flags,
                 int fd, __off_t offset){

  print_strace("[runtime] [mmap]: addr: %p, length %lu, prot 0x%x, flags 0x%x, fd %i, offset %lu\r\n", addr, length, prot, flags, fd, offset);
  return (uintptr_t)((void*)-1);
}
