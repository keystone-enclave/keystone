#ifdef IO_SYSCALL_WRAPPING
#include <stdint.h>
#include "io_wrap.h"
#include <alloca.h>
#include "uaccess.h"
#include "syscall.h"
#include "string.h"
#include "edge_syscall.h"

/* Syscalls iozone uses in -i0 mode
*** Fake these
 *   uname
*** odd
    rt_sigaction
    rt_sigprocmask
*** hard
    brk
    mmap
*/

#define MAX_STRACE_PRINT 20

uintptr_t io_syscall_sync(){
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();

  edge_syscall->syscall_num = SYS_sync;

  size_t totalsize = (sizeof(struct edge_syscall));

  uintptr_t ret = dispatch_edgecall_syscall(edge_syscall, totalsize);
  print_strace("[runtime] proxied sync\r\n");
  return ret;
}

uintptr_t io_syscall_ftruncate(int fd, off_t offset){
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_ftruncate* args = (sargs_SYS_ftruncate*)edge_syscall->data;
  edge_syscall->syscall_num = SYS_ftruncate;

  args->fd = fd;
  args->offset = offset;

  size_t totalsize = (sizeof(struct edge_syscall)+
                      sizeof(sargs_SYS_ftruncate));

  uintptr_t ret = dispatch_edgecall_syscall(edge_syscall, totalsize);
  print_strace("[runtime] proxied ftruncate (%i) = %li\r\n", fd, ret);
  return ret;
}
uintptr_t io_syscall_fsync(int fd){
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_fsync* args = (sargs_SYS_fsync*)edge_syscall->data;
  edge_syscall->syscall_num = SYS_fsync;

  args->fd = fd;

  size_t totalsize = (sizeof(struct edge_syscall)+
                      sizeof(sargs_SYS_fsync));

  uintptr_t ret = dispatch_edgecall_syscall(edge_syscall, totalsize);
  print_strace("[runtime] proxied fsync (%i) = %li\r\n", fd, ret);
  return ret;
}

uintptr_t io_syscall_lseek(int fd, off_t offset, int whence){
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_lseek* args = (sargs_SYS_lseek*)edge_syscall->data;
  edge_syscall->syscall_num = SYS_lseek;

  args->fd = fd;
  args->offset = offset;
  args->whence = whence;

  size_t totalsize = (sizeof(struct edge_syscall)+
                      sizeof(sargs_SYS_lseek));

  uintptr_t ret = dispatch_edgecall_syscall(edge_syscall, totalsize);
  print_strace("[runtime] proxied lseek (on fd:%i to %li from %i) = %li\r\n",
               fd, offset, whence, ret);
  return ret;
}

uintptr_t io_syscall_close(int fd){
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_close* args = (sargs_SYS_close*)edge_syscall->data;
  edge_syscall->syscall_num = SYS_close;

  args->fd = fd;

  size_t totalsize = (sizeof(struct edge_syscall)+
                      sizeof(sargs_SYS_close));

  uintptr_t ret = dispatch_edgecall_syscall(edge_syscall, totalsize);
  print_strace("[runtime] proxied close (%i) = %li\r\n", fd, ret);
  return ret;
}

uintptr_t io_syscall_read(int fd, void* buf, size_t len){
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_read* args = (sargs_SYS_read*)edge_syscall->data;
  uintptr_t ret = -1;
  edge_syscall->syscall_num = SYS_read;
  args->fd =fd;
  args->len = len;

  // Sanity check that the read buffer will fit in the shared memory
  if(edge_call_check_ptr_valid((uintptr_t)args->buf, len) != 0){
    goto done;
  }

  size_t totalsize = (sizeof(struct edge_syscall) +
                      sizeof(sargs_SYS_read) +
                      len);

  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  if((int)ret < 0){
    goto done;
  }

  // Previously checked that this is staying in untrusted buffer range
  copy_to_user(buf, args->buf, ret > len? len: ret);

 done:
  print_strace("[runtime] proxied read (size: %lu) = %li\r\n",len, ret);
  return ret;
}


uintptr_t io_syscall_write(int fd, void* buf, size_t len){
  /* print_strace("[write] len :%lu\r\n", len); */
  /* if(len > 0){ */
  /*   size_t stracelen = len > MAX_STRACE_PRINT? MAX_STRACE_PRINT:len; */
  /*   char* lbuf[MAX_STRACE_PRINT+1]; */
  /*   memset(lbuf, 0, sizeof(lbuf)); */
  /*   copy_from_user(lbuf, (void*)buf, stracelen); */
  /*   print_strace("[write] \"%s\"\r\n", (char*)lbuf); */
  /* } */

  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_write* args = (sargs_SYS_write*)edge_syscall->data;
  uintptr_t ret = -1;

  edge_syscall->syscall_num = SYS_write;
  args->fd =fd;
  args->len = len;

  // Sanity check that the write buffer will fit in the shared memory
  if(edge_call_check_ptr_valid((uintptr_t)args->buf, len) != 0){
    goto done;
  }

  copy_from_user(args->buf, buf, len);

  size_t totalsize = (sizeof(struct edge_syscall) +
                      sizeof(sargs_SYS_write) +
                      len);

  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

 done:
  print_strace("[runtime] proxied write (size: %lu) = %li\r\n",len, ret);
  return ret;
}

uintptr_t io_syscall_openat(int dirfd, char* path,
                            int flags, mode_t mode){
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_openat* args = (sargs_SYS_openat*)edge_syscall->data;

  edge_syscall->syscall_num = SYS_openat;
  args->dirfd = dirfd;
  args->flags = flags;
  args->mode = mode;
  uintptr_t ret = -1;

  size_t pathlen;
  ALLOW_USER_ACCESS(pathlen = _strlen(path)+1);
  // Sanity check that the buffer will fit in the shared memory
  if(edge_call_check_ptr_valid((uintptr_t)args->path, pathlen) != 0){
    goto done;
  }
  copy_from_user(args->path, path, pathlen);

  size_t totalsize = (sizeof(struct edge_syscall) +
                      sizeof(sargs_SYS_openat) +
                      pathlen);

  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);


 done:
  // TODO path print here isn't necessarily correct or even copied!
  print_strace("[runtime] proxied openat(path: %.*s) = %li\r\n",
               pathlen>MAX_STRACE_PRINT?MAX_STRACE_PRINT:pathlen,args->path, ret);

  return ret;
}

uintptr_t io_syscall_unlinkat(int dirfd, char* path,
                              int flags){
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_unlinkat* args = (sargs_SYS_unlinkat*)edge_syscall->data;
  uintptr_t ret = -1;

  edge_syscall->syscall_num = SYS_unlinkat;
  args->dirfd = dirfd;
  args->flags = flags;
  size_t pathlen;
  ALLOW_USER_ACCESS(pathlen = _strlen(path)+1);
  // Sanity check that the buffer will fit in the shared memory
  if(edge_call_check_ptr_valid((uintptr_t)args->path, pathlen) != 0){
    goto done;
  }
  copy_from_user(args->path, path, pathlen);

  size_t totalsize = (sizeof(struct edge_syscall) +
                      sizeof(sargs_SYS_unlinkat) +
                      pathlen);

  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);


 done:
  // TODO path print here isn't necessarily correct or even copied!
  print_strace("[runtime] proxied unlinkat(path: %.*s) = %li\r\n",
               pathlen>MAX_STRACE_PRINT?MAX_STRACE_PRINT:pathlen,args->path, ret);
  return ret;
}

uintptr_t io_syscall_writev(int fd, const struct iovec *iov, int iovcnt){
  int i=0;
  uintptr_t ret = 0;
  size_t total = 0;
  print_strace("[runtime] Simulating writev (cnt %i) with write calls\r\n",iovcnt);
  for(i=0; i<iovcnt && ret >= 0;i++){
    struct iovec iov_local;
    copy_from_user(&iov_local, &(iov[i]), sizeof(struct iovec));
    ret = io_syscall_write(fd,iov_local.iov_base, iov_local.iov_len);
    total += ret;
  }
  ret = total;
  print_strace("[runtime] Simulated writev = %li\r\n",ret);
  return ret;
}

uintptr_t io_syscall_readv(int fd, const struct iovec *iov, int iovcnt){
  int i=0;
  uintptr_t ret = 0;
  size_t total = 0;
  print_strace("[runtime] Simulating readv (cnt %i) with read calls\r\n",iovcnt);
  for(i=0; i<iovcnt && ret >= 0;i++){
    struct iovec iov_local;
    copy_from_user(&iov_local, &(iov[i]), sizeof(struct iovec));
    ret = io_syscall_read(fd, iov_local.iov_base, iov_local.iov_len);
    total += ret;
  }

  ret = total;
  print_strace("[runtime] Simulated readv = %li\r\n",ret);
  return ret;
}

uintptr_t io_syscall_fstatat(int dirfd, char *pathname, struct stat *statbuf,
                                int flags){
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_fstatat* args = (sargs_SYS_fstatat*)edge_syscall->data;
  uintptr_t ret = -1;

  edge_syscall->syscall_num = SYS_fstatat;
  args->dirfd = dirfd;
  args->flags = flags;

  size_t pathlen;
  ALLOW_USER_ACCESS(pathlen = _strlen(pathname)+1);
  // Sanity check that the buffer will fit in the shared memory
  if(edge_call_check_ptr_valid((uintptr_t)args->pathname, pathlen) != 0){
    goto done;
  }
  copy_from_user(args->pathname, pathname, pathlen);

  size_t totalsize = (sizeof(struct edge_syscall) +
                      sizeof(sargs_SYS_fstatat) +
                      pathlen);

  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  if(ret == 0){
    copy_to_user(statbuf, &args->stats, sizeof(struct stat));
  }

 done:
  print_strace("[runtime] proxied fstatat (path %.*s) = %li\r\n",
               pathlen>MAX_STRACE_PRINT?MAX_STRACE_PRINT:pathlen,args->pathname, ret);
  return ret;

}
#endif /* IO_SYSCALL_WRAPPING */
