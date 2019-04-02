#ifndef _MM_H_
#define _MM_H_

uintptr_t remap_physical_page(uintptr_t vpn, uintptr_t ppn, int flags);
uintptr_t remap_physical_range(uintptr_t vpn, uintptr_t ppn, size_t count, int flags);
uintptr_t translate(uintptr_t va);

uintptr_t alloc_page(uintptr_t vpn, int flags);
uintptr_t alloc_pages(uintptr_t vpn, size_t count, int flags);

uintptr_t syscall_mmap(void *addr, size_t length, int prot, int flags,
                  int fd, __off_t offset);

#endif /* _MM_H_ */
