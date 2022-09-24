#include "edge_syscall.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/sendfile.h>
// Special edge-call handler for syscall proxying
void
incoming_syscall(struct edge_call* edge_call) {
  struct edge_syscall* syscall_info;

  size_t args_size;

  if (edge_call_args_ptr(edge_call, (uintptr_t*)&syscall_info, &args_size) != 0)
    goto syscall_error;

  // NOTE: Right now we assume that the args data is safe, even though
  // it may be changing under us. This should be safer in the future.

  edge_call->return_data.call_status = CALL_STATUS_OK;

  int64_t ret;
  int is_str_ret = 0; 
  char* retbuf;

  // Right now we only handle some io syscalls. See runtime for how
  // others are handled.
  switch (syscall_info->syscall_num) {
    case (SYS_openat):;
      sargs_SYS_openat* openat_args = (sargs_SYS_openat*)syscall_info->data;
      ret                           = openat(
          openat_args->dirfd, openat_args->path, openat_args->flags,
          openat_args->mode);
      break;
    case (SYS_unlinkat):;
      sargs_SYS_unlinkat* unlinkat_args =
          (sargs_SYS_unlinkat*)syscall_info->data;
      ret = unlinkat(
          unlinkat_args->dirfd, unlinkat_args->path, unlinkat_args->flags);
      break;
    case (SYS_ftruncate):;
      sargs_SYS_ftruncate* ftruncate_args =
          (sargs_SYS_ftruncate*)syscall_info->data;
      ret = ftruncate(ftruncate_args->fd, ftruncate_args->offset);
      break;
    case (SYS_fstatat):;
      sargs_SYS_fstatat* fstatat_args = (sargs_SYS_fstatat*)syscall_info->data;
      // Note the use of the implicit buffer in the stat args object (stats)
      ret = fstatat(
          fstatat_args->dirfd, fstatat_args->pathname, &fstatat_args->stats,
          fstatat_args->flags);
			break;
    case (SYS_fstat):; 
      sargs_SYS_fstat* fstat_args = (sargs_SYS_fstat*)syscall_info->data;
      // Note the use of the implicit buffer in the stat args object (stats)
			ret = fstat(fstat_args->fd, &fstat_args->stats);
			break;
    case (SYS_getcwd):;  // TODO: how to handle string return 
      sargs_SYS_getcwd* getcwd_args = (sargs_SYS_getcwd*)syscall_info->data;
			retbuf = getcwd(getcwd_args->buf, getcwd_args->size);
      is_str_ret = 1;
			break;
    case (SYS_write):;
      sargs_SYS_write* write_args = (sargs_SYS_write*)syscall_info->data;
      ret = write(write_args->fd, write_args->buf, write_args->len);
      break;
    case (SYS_read):;
      sargs_SYS_read* read_args = (sargs_SYS_read*)syscall_info->data;
      ret = read(read_args->fd, read_args->buf, read_args->len);
      break;
    case (SYS_sync):;
      sync();
      ret = 0;
      break;
    case (SYS_fsync):;
      sargs_SYS_fsync* fsync_args = (sargs_SYS_fsync*)syscall_info->data;
      ret                         = fsync(fsync_args->fd);
      break;
    case (SYS_close):;
      sargs_SYS_close* close_args = (sargs_SYS_close*)syscall_info->data;
      ret                         = close(close_args->fd);
      break;
    case (SYS_lseek):;
      sargs_SYS_lseek* lseek_args = (sargs_SYS_lseek*)syscall_info->data;
      ret = lseek(lseek_args->fd, lseek_args->offset, lseek_args->whence);
      break;
    case (SYS_pipe2):;
      int *fds = (int *) syscall_info->data;
      ret = pipe(fds); 
      break;
    case (SYS_epoll_create1):;
      sargs_SYS_epoll_create1 *epoll_args = (sargs_SYS_epoll_create1 *) syscall_info->data;
      ret = epoll_create(epoll_args->size);
      break;
    case(SYS_chdir):;
      sargs_SYS_chdir* chdir_args = (sargs_SYS_chdir*) syscall_info->data;
			ret = chdir(chdir_args->path);
			break;
    case (SYS_epoll_ctl):;
      sargs_SYS_epoll_ctl *epoll_ctl_args = (sargs_SYS_epoll_ctl *) syscall_info->data;
      ret = epoll_ctl(epoll_ctl_args->epfd, epoll_ctl_args->op, epoll_ctl_args->fd, (struct epoll_event * ) &epoll_ctl_args->event);
      break;
    case (SYS_epoll_pwait):;
      sargs_SYS_epoll_pwait *epoll_pwait_args = (sargs_SYS_epoll_pwait *) syscall_info->data;
      ret = epoll_wait(epoll_pwait_args->epfd, &epoll_pwait_args->events, 
                epoll_pwait_args->maxevents, epoll_pwait_args->timeout);
      break;
    case (SYS_getpeername):;
      sargs_SYS_getpeername *getpeername_args = (sargs_SYS_getpeername *) syscall_info->data;
      ret = getpeername(getpeername_args->sockfd, (struct sockaddr *) &getpeername_args->addr, 
                &getpeername_args->addrlen);

      break; 
    case (SYS_getsockname):; 
      sargs_SYS_getsockname *getsockname_args = (sargs_SYS_getsockname *) syscall_info->data;
      ret = getsockname(getsockname_args->sockfd, (struct sockaddr *) &getsockname_args->addr, 
                &getsockname_args->addrlen);
      break; 
    case (SYS_renameat2):;
      sargs_SYS_renameat2 *renameat_args = (sargs_SYS_renameat2 *) syscall_info->data;

      ret = renameat(renameat_args->olddirfd, renameat_args->oldpath, 
                renameat_args->newdirfd, renameat_args->newpath);
      break; 
    case (SYS_umask):;
      sargs_SYS_umask *umask_args = (sargs_SYS_umask *) syscall_info->data;
      ret = umask(umask_args->mask);
      break; 
    case (SYS_socket):;
      sargs_SYS_socket *socket_args = (sargs_SYS_socket *) syscall_info->data; 
      ret = socket(socket_args->domain, socket_args->type, socket_args->protocol); 
      break; 
    case (SYS_setsockopt):;
      sargs_SYS_setsockopt *setsockopt_args = (sargs_SYS_setsockopt *) syscall_info->data; 
      ret = setsockopt(setsockopt_args->socket, setsockopt_args->level, setsockopt_args->option_name, &setsockopt_args->option_value, setsockopt_args->option_len);
      break;
    case (SYS_bind):;
      sargs_SYS_bind *bind_args = (sargs_SYS_bind *) syscall_info->data; 
      ret = bind(bind_args->sockfd, (struct sockaddr *) &bind_args->addr, bind_args->addrlen);
      break;
    case (SYS_listen):;
      sargs_SYS_listen *listen_args = (sargs_SYS_listen *) syscall_info->data; 
      ret = listen(listen_args->sockfd, listen_args->backlog);
      break;
    case (SYS_accept):;
      sargs_SYS_accept *accept_args = (sargs_SYS_accept *) syscall_info->data; 
      ret = accept(accept_args->sockfd, (struct sockaddr *) &accept_args->addr, &accept_args->addrlen);
      break;
    case (SYS_recvfrom):; 
      sargs_SYS_recvfrom *recvfrom_args = (sargs_SYS_recvfrom *) syscall_info->data; 
      struct sockaddr *src_addr = recvfrom_args->src_addr_is_null ? NULL : &recvfrom_args->src_addr; 
      socklen_t *addrlen = recvfrom_args->src_addr_is_null ? NULL : &recvfrom_args->addrlen; 
      ret = recvfrom(recvfrom_args->sockfd, recvfrom_args->buf, recvfrom_args->len, recvfrom_args->flags, 
                      src_addr, addrlen);
      break;
    case (SYS_sendto):; 
      sargs_SYS_sendto *sendto_args = (sargs_SYS_sendto *) syscall_info->data; 
      struct sockaddr *dest_addr = sendto_args->dest_addr_is_null ? NULL : &sendto_args->dest_addr; 
      socklen_t dest_addrlen = sendto_args->dest_addr_is_null ? 0 : sendto_args->addrlen; 
      ret = sendto(sendto_args->sockfd, sendto_args->buf, sendto_args->len, sendto_args->flags, 
                      dest_addr, dest_addrlen);
      break;
    case (SYS_sendfile):; 
      sargs_SYS_sendfile *sendfile_args = (sargs_SYS_sendfile *) syscall_info->data; 
      ret = sendfile(sendfile_args->out_fd, sendfile_args->in_fd, &sendfile_args->offset, sendfile_args->count);
      break;
    case (SYS_fcntl):;
      sargs_SYS_fcntl* fcntl_args = (sargs_SYS_fcntl*)syscall_info->data; 
      if (!fcntl_args->has_struct) 
        ret = fcntl(fcntl_args->fd, fcntl_args->cmd, fcntl_args->arg[0]);
      else 
        ret = fcntl(fcntl_args->fd, fcntl_args->cmd, fcntl_args->arg);
      break; 
    case (SYS_getuid):;
      ret = getuid(); 
      break;
    case (SYS_pselect6):;
      sargs_SYS_pselect* pselect_args = (sargs_SYS_pselect*)syscall_info->data; 
      fd_set *readfds = pselect_args->readfds_is_null ? NULL : &pselect_args->readfds; 
      fd_set *writefds = pselect_args->writefds_is_null ? NULL : &pselect_args->writefds; 
      fd_set *exceptfds = pselect_args->exceptfds_is_null ? NULL : &pselect_args->exceptfds; 
      struct timespec *timeout = pselect_args->timeout_is_null ? NULL : &pselect_args->timeout; 
      sigset_t *sigmask = pselect_args->sigmask_is_null ? NULL : &pselect_args->sigmask; 
      ret = pselect(pselect_args->nfds, readfds, writefds, exceptfds, timeout, sigmask);
      break;
    default:
      goto syscall_error;
  }

  /* Setup return value */
  void* ret_data_ptr      = (void*)edge_call_data_ptr();
  if (is_str_ret) {
    *(char**) ret_data_ptr = retbuf; // TODO: check ptr stuff
    if (edge_call_setup_ret(edge_call, ret_data_ptr, sizeof(int64_t)) != 0)
      goto syscall_error;
  } else {
    *(int64_t*)ret_data_ptr = ret;
    if (edge_call_setup_ret(edge_call, ret_data_ptr, sizeof(int64_t)) != 0)
      goto syscall_error;
  }

  return;

syscall_error:
  edge_call->return_data.call_status = CALL_STATUS_SYSCALL_FAILED;
  return;
}
