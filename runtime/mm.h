#ifndef _MM_H_
#define _MM_H_

uintptr_t syscall_mmap(void *addr, size_t length, int prot, int flags,
                  int fd, __off_t offset);

#endif /* _MM_H_ */
