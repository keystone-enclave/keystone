#ifdef IO_SYSCALL_WRAPPING
#ifndef _IO_WRAP_H_
#define _IO_WRAP_H_

#include <sys/uio.h>
#include <sys/types.h>
#include "edge_syscall.h"

uintptr_t io_syscall_read(int fd, void* buf, size_t len);
uintptr_t io_syscall_write(int fd, void* buf, size_t len);
uintptr_t io_syscall_writev(int fd, const struct iovec *iov, int iovcnt);
uintptr_t io_syscall_readv(int fd, const struct iovec *iov, int iovcnt);
uintptr_t io_syscall_openat(int dirfd, char* path,
                            int flags, mode_t mode);
uintptr_t io_syscall_fstatat(int dirfd, char *pathname, struct stat *statbuf,
                             int flags);
uintptr_t io_syscall_unlinkat(int dirfd, char* path,
                              int flags);
uintptr_t io_syscall_fsync(int fd);
uintptr_t io_syscall_close(int fd);
uintptr_t io_syscall_sync();
uintptr_t io_syscall_lseek(int fd, off_t offset, int whence);
uintptr_t io_syscall_ftruncate(int fd, off_t length);
#endif /* _IO_WRAP_H_ */
#endif /* IO_SYSCALL_WRAPPING */
