#ifdef USE_IO_SYSCALL
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
uintptr_t io_syscall_fstat(int fd, struct stat *statbuf);
uintptr_t io_syscall_unlinkat(int dirfd, char* path,
                              int flags);
uintptr_t io_syscall_fsync(int fd);
uintptr_t io_syscall_close(int fd);
uintptr_t io_syscall_sync();
uintptr_t io_syscall_lseek(int fd, off_t offset, int whence);
uintptr_t io_syscall_ftruncate(int fd, off_t length);
uintptr_t io_syscall_pipe(int *fds);
uintptr_t io_syscall_epoll_create(int size); 
uintptr_t io_syscall_fcntl(int fd, int cmd, uintptr_t arg);
uintptr_t io_syscall_epoll_ctl(int epfd, int op, int fd, uintptr_t event);
uintptr_t io_syscall_getcwd(char* buf, size_t size);
uintptr_t io_syscall_chdir(char* path);
uintptr_t io_syscall_epoll_pwait(int epfd, uintptr_t events, int maxevents, int timeout);
uintptr_t io_syscall_renameat2(int olddirfd,  uintptr_t oldpath, int newdirfd, uintptr_t newpath, unsigned int flags);
uintptr_t io_syscall_umask(int mask);
#endif /* _IO_WRAP_H_ */
#endif /* USE_IO_SYSCALL */
