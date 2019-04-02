#include "edge_syscall.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
// Special edge-call handler for syscall proxying
void incoming_syscall(edge_call_t* edge_call){



  edge_syscall_t* syscall_info;

  if( edge_call_args_ptr(edge_call, (uintptr_t*)&syscall_info) != 0)
    goto syscall_error;

  edge_call->return_data.call_status = CALL_STATUS_OK;

  int64_t ret;

  // Right now we only handle some io syscalls. See runtime for how
  // others are handled.
  switch(syscall_info->syscall_num){

  case(SYS_openat):;
    sargs_SYS_openat* openat_args = (sargs_SYS_openat*)syscall_info->data;
    ret = openat(openat_args->dirfd, openat_args->path,
                 openat_args->flags, openat_args->mode);
    break;
  case(SYS_unlinkat):;
    sargs_SYS_unlinkat* unlinkat_args = (sargs_SYS_unlinkat*)syscall_info->data;
    ret = unlinkat(unlinkat_args->dirfd, unlinkat_args->path,
                   unlinkat_args->flags);
    break;
  case(SYS_ftruncate):;
    sargs_SYS_ftruncate* ftruncate_args = (sargs_SYS_ftruncate*)syscall_info->data;
    ret = ftruncate(ftruncate_args->fd, ftruncate_args->offset);
    break;
  case(SYS_fstatat):;
    sargs_SYS_fstatat* fstatat_args = (sargs_SYS_fstatat*)syscall_info->data;
    // Note the use of the implicit buffer in the stat args object (stats)
    ret = fstatat(fstatat_args->dirfd, fstatat_args->pathname,
                  &fstatat_args->stats, fstatat_args->flags);
    break;
  case(SYS_write):;
    sargs_SYS_write* write_args = (sargs_SYS_write*)syscall_info->data;
    ret = write(write_args->fd, write_args->buf, write_args->len);
    break;
  case(SYS_read):;
    sargs_SYS_read* read_args = (sargs_SYS_read*)syscall_info->data;
    ret = read(read_args->fd, read_args->buf, read_args->len);
    break;
  case(SYS_sync):;
    sync();
    ret = 0;
    break;
  case(SYS_fsync):;
    sargs_SYS_fsync* fsync_args = (sargs_SYS_fsync*)syscall_info->data;
    ret = fsync(fsync_args->fd);
    break;
  case(SYS_close):;
    sargs_SYS_close* close_args = (sargs_SYS_close*)syscall_info->data;
    ret = close(close_args->fd);
    break;
  case(SYS_lseek):;
    sargs_SYS_lseek* lseek_args = (sargs_SYS_lseek*)syscall_info->data;
    ret = lseek(lseek_args->fd, lseek_args->offset, lseek_args->whence);
    break;
  default:
    goto syscall_error;
  }

  /* Setup return value */
  void* ret_data_ptr = (void*)edge_call_data_ptr();
  *(int64_t*)ret_data_ptr = ret;
  if(edge_call_setup_ret(edge_call, ret_data_ptr , sizeof(int64_t)) !=0) goto syscall_error;

  return;

 syscall_error:
  edge_call->return_data.call_status = CALL_STATUS_SYSCALL_FAILED;
  return;
}
