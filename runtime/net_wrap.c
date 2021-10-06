#ifdef NET_SYSCALL_WRAPPING
#include <stdint.h>
#include "io_wrap.h"
#include <alloca.h>
#include "uaccess.h"
#include "syscall.h"
#include "string.h"
#include "edge_syscall.h"
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/select.h>

//Length of optional value for setsockopt 
#define MAX_OPTION_LEN 256

uintptr_t io_syscall_socket(int domain, int type, int protocol){
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  edge_syscall->syscall_num = SYS_socket;

  sargs_SYS_socket *args = (sargs_SYS_socket *) edge_syscall->data;

  args->domain = domain; 
  args->type = type; 
  args->protocol = protocol; 

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_socket);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied socket: %d \r\n", ret);
  return ret; 
}

uintptr_t io_syscall_setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len){
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  edge_syscall->syscall_num = SYS_setsockopt;

  sargs_SYS_setsockopt *args = (sargs_SYS_setsockopt *) edge_syscall->data;

  args->socket = socket; 
  args->level = level; 
  args->option_name = option_name; 
  args->option_len = option_len; 

  if(option_len > MAX_OPTION_LEN){
    return ret; 
  }

  copy_from_user(&args->option_value, option_value, option_len);

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_setsockopt);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied setsockopt: %d \r\n", ret);
  return ret; 

}

uintptr_t io_syscall_bind (int sockfd, uintptr_t addr, socklen_t addrlen){

  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  edge_syscall->syscall_num = SYS_bind;

  sargs_SYS_bind *args = (sargs_SYS_bind *) edge_syscall->data;

  args->sockfd = sockfd; 
  args->addrlen = addrlen; 

  if(addrlen > sizeof(struct sockaddr_storage)) {
    return -1; 
  }

  copy_from_user(&args->addr, (void *) addr, addrlen);

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_bind);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied bind: %d \r\n", ret);
  return ret; 

}

uintptr_t io_syscall_listen(int sockfd, int backlog){
   uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  edge_syscall->syscall_num = SYS_listen;

  sargs_SYS_listen *args = (sargs_SYS_listen *) edge_syscall->data;

  args->sockfd = sockfd; 
  args->backlog = backlog; 

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_listen);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied listen: %d \r\n", ret);
  return ret; 

}

uintptr_t io_syscall_accept(int sockfd, uintptr_t addr, uintptr_t addrlen) {
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  edge_syscall->syscall_num = SYS_accept;

  sargs_SYS_accept *args = (sargs_SYS_accept *) edge_syscall->data;

  args->sockfd = sockfd; 

  if(addrlen > sizeof(struct sockaddr_storage)) {
    return ret; 
  }

  copy_from_user(&args->addrlen, (void *) addrlen, sizeof(socklen_t));
  copy_from_user(&args->addr, (void *) addr, args->addrlen);

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_accept);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied accept: %d \r\n", ret);
  return ret; 
}

uintptr_t io_syscall_getpeername(int sockfd, uintptr_t addr,
                       uintptr_t addrlen){
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_getpeername* args = (sargs_SYS_getpeername*) edge_syscall->data;

  edge_syscall->syscall_num = SYS_getpeername;
  args->sockfd = sockfd;

  copy_from_user(&args->addrlen, (void *) addrlen, sizeof(socklen_t)); 
  copy_from_user(&args->addr, (void *) addr, args->addrlen);  


  size_t totalsize = (sizeof(struct edge_syscall)) + sizeof(sargs_SYS_getpeername);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied getpeername: fd: %d, ret: %d\r\n", args->sockfd, ret);
  return ret;
}

uintptr_t io_syscall_getsockname(int sockfd, uintptr_t addr,
                       uintptr_t addrlen){
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_getsockname* args = (sargs_SYS_getsockname*) edge_syscall->data;

  edge_syscall->syscall_num = SYS_getsockname;
  args->sockfd = sockfd;

  copy_from_user(&args->addrlen, (void *) addrlen, sizeof(socklen_t)); 
  copy_from_user(&args->addr, (void *) addr, args->addrlen);  

  size_t totalsize = (sizeof(struct edge_syscall)) + sizeof(sargs_SYS_getsockname);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied getsockname: fd: %d, ret: %d\r\n", args->sockfd, ret);
  return ret;
}

uintptr_t io_syscall_getuid() {
  uintptr_t ret = -1; 
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();

  edge_syscall->syscall_num = SYS_getuid;

  size_t totalsize = (sizeof(struct edge_syscall));
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied getuid, ret: %d\r\n", ret);
  return ret;
}

uintptr_t io_syscall_pselect(int nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, uintptr_t timeout,
            uintptr_t sigmask) {
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_pselect* args = (sargs_SYS_pselect*) edge_syscall->data;

  edge_syscall->syscall_num = SYS_pselect6;
  args->nfds = nfds; 

  if (readfds != (void *) 0) 
    copy_from_user(&args->readfds, (void *) readfds, sizeof(fd_set)); 
  if (writefds != (void *) 0)
    copy_from_user(&args->writefds, (void *) writefds, sizeof(fd_set)); 
  if (exceptfds != (void *) 0)
    copy_from_user(&args->exceptfds, (void *) exceptfds, sizeof(fd_set)); 
  if (timeout != 0) 
    copy_from_user(&args->timeout, (void *) timeout, sizeof(struct timespec)); 
  if (sigmask != 0) 
    copy_from_user(&args->sigmask, (void *) sigmask, sizeof(sigset_t));  

  size_t totalsize = (sizeof(struct edge_syscall)) + sizeof(sargs_SYS_pselect);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  if (ret >= 0) {
    if (readfds != (void *) 0) 
      copy_to_user(readfds, &args->readfds, sizeof(fd_set)); 
    if (writefds != (void *) 0)
      copy_to_user(writefds, &args->writefds, sizeof(fd_set)); 
    if (exceptfds != (void *) 0)
      copy_to_user(exceptfds, &args->exceptfds, sizeof(fd_set));
  }

  print_strace("[runtime] proxied pselect: nfds: %d, ret: %d\r\n", args->nfds, ret);
  return ret;
}

#endif /* NET_SYSCALL_WRAPPING */ 