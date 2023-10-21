#ifdef USE_NET_SYSCALL
#include <stdint.h>
#include "call/io_wrap.h"
#include <alloca.h>
#include "uaccess.h"
#include "call/syscall.h"
#include "util/string.h"
#include "edge_syscall.h"
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/select.h>
#include <poll.h>

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

  sargs_SYS_getsetsockopt *args = (sargs_SYS_getsetsockopt *) edge_syscall->data;

  args->socket = socket; 
  args->level = level; 
  args->option_name = option_name; 
  args->option_len = option_len; 

  if(option_len > MAX_OPTION_LEN){
    return ret; 
  }

  copy_from_user(&args->option_value, option_value, option_len);

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_getsetsockopt);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied setsockopt: %d \r\n", ret);
  return ret; 

}

uintptr_t io_syscall_getsockopt(int socket, int level, int optname, uintptr_t optval, uintptr_t optlen){
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  edge_syscall->syscall_num = SYS_getsockopt;

  sargs_SYS_getsetsockopt *args = (sargs_SYS_getsetsockopt *) edge_syscall->data;

  args->socket = socket;
  args->level = level;
  args->option_name = optname;

  // First, copy the available length
  copy_from_user(&args->option_len, (void *) optlen, sizeof(socklen_t));

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_getsetsockopt);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  // Copy back the actual length and the data
  // todo assert that size back is actually valid

  copy_to_user((void *) optlen, &args->option_len, sizeof(socklen_t));
  copy_to_user((void *) optval, &args->option_value, args->option_len);

  print_strace("[runtime] proxied getsockopt: %d \r\n", ret);
  return ret;
}

uintptr_t io_syscall_connect(int sockfd, uintptr_t addr, socklen_t addrlen){
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  edge_syscall->syscall_num = SYS_connect;

  sargs_SYS_connect *args = (sargs_SYS_connect *) edge_syscall->data;

  args->sockfd = sockfd;
  args->addrlen = addrlen;

  if(addrlen > sizeof(struct sockaddr_storage)) {
    return -1;
  }

  copy_from_user(&args->addr, (void *) addr, addrlen);
  
  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_connect);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied connect: %d \r\n", ret);
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

  copy_from_user(&args->addrlen, (void *) addrlen, sizeof(socklen_t));
  copy_from_user(&args->addr, (void *) addr, args->addrlen);

  if(args->addrlen > sizeof(struct sockaddr_storage)) {
    return ret; 
  }

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_accept);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  print_strace("[runtime] proxied accept: %d \r\n", ret);
  return ret; 
}

uintptr_t io_syscall_recvfrom(int sockfd, uintptr_t buf, size_t len, int flags,
                				uintptr_t src_addr, uintptr_t addrlen) {
	uintptr_t ret = -1;
	struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
	edge_syscall->syscall_num = SYS_recvfrom;

	sargs_SYS_recvfrom *args = (sargs_SYS_recvfrom *) edge_syscall->data;

	args->sockfd = sockfd; 
	args->len = len;
	args->flags = flags; 

	/* If src_addr is NULL, then addrlen is not used */
	if (src_addr != 0) {
		args->src_addr_is_null = 0; 
		if(edge_call_check_ptr_valid((uintptr_t)&args->addrlen, sizeof(socklen_t)) != 0){
			goto done;
		}
		copy_from_user(&args->addrlen, (void *) addrlen, sizeof(socklen_t));
		if(edge_call_check_ptr_valid((uintptr_t)&args->src_addr, args->addrlen) != 0){
			goto done;
		}
		copy_from_user(&args->src_addr, (void *) src_addr, args->addrlen);

		if(args->addrlen > sizeof(struct sockaddr)) {
			goto done;
		}
	} else {
		args->src_addr_is_null = 1; 
	}

	if(edge_call_check_ptr_valid((uintptr_t)args->buf, len) != 0){
    	goto done;
  	}
	size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_recvfrom);
	ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

	if (ret > 0) {
		copy_to_user((void *) buf, &args->buf, ret);
    if (src_addr != 0) {
      copy_to_user((void*)src_addr, &args->src_addr, args->addrlen);
      copy_to_user((void*)addrlen, &args->addrlen, sizeof(socklen_t));
    }
	}

	done: 
		print_strace("[runtime] proxied recvfrom: %d \r\n", ret);
		return ret; 
}

uintptr_t io_syscall_sendto(int sockfd, uintptr_t buf, size_t len, int flags,
                				uintptr_t dest_addr, int addrlen) {
	uintptr_t ret = -1;
	struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
	edge_syscall->syscall_num = SYS_sendto;

	sargs_SYS_sendto *args = (sargs_SYS_sendto *) edge_syscall->data;

	args->sockfd = sockfd; 
	args->len = len;
	args->flags = flags; 

	/* If dest_addr is NULL, then addrlen is not used */
	if (dest_addr != 0) {
		args->dest_addr_is_null = 0; 
		args->addrlen = addrlen;
		if(edge_call_check_ptr_valid((uintptr_t)&args->dest_addr, args->addrlen) != 0){
			goto done;
		}
		copy_from_user(&args->dest_addr, (void *) dest_addr, args->addrlen);

		if(args->addrlen > sizeof(struct sockaddr)) {
			goto done;
		}
	} else {
		args->dest_addr_is_null = 1; 
	}

	/* Copy message in buf to untrusted memory */
	if(edge_call_check_ptr_valid((uintptr_t)args->buf, len) != 0){
    	goto done;
  	}	
	copy_from_user(&args->buf, (void *) buf, len);
	print_strace("[runtime] sendto buf: %s \r\n", &args->buf);

	size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_sendto);
	ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

	done: 
		print_strace("[runtime] proxied sendto: %d \r\n", ret);
		return ret; 
}

uintptr_t io_syscall_sendfile(int out_fd, int in_fd, uintptr_t offset, int count) {
	uintptr_t ret = -1;
	struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
	edge_syscall->syscall_num = SYS_sendfile;

	sargs_SYS_sendfile *args = (sargs_SYS_sendfile *) edge_syscall->data;

	args->out_fd = out_fd; 
	args->in_fd = in_fd; 
	args->count = count; 

	/* If offset is NULL, don't copy */
	if (offset != 0) {
		args->offset_is_null = 0; 
		if(edge_call_check_ptr_valid((uintptr_t)&args->offset, sizeof(off_t)) != 0){
			goto done;
		}
		copy_from_user(&args->offset, (void *) offset, sizeof(off_t));
	} else {
		args->offset_is_null = 1; 
	}

	size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_sendfile);
	ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

	/* Offset value is updated to reflect new position */
	copy_to_user((void *) offset, &args->offset, sizeof(off_t)); 

	done: 
		print_strace("[runtime] proxied sendfile: %d \r\n", ret);
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

  copy_to_user((void *) addr, &args->addr, args->addrlen);
  copy_to_user((void *) addrlen, &args->addrlen, sizeof(socklen_t));

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

uintptr_t io_syscall_pselect(int nfds, uintptr_t readfds, uintptr_t writefds,
                              uintptr_t exceptfds, uintptr_t timeout,
                              uintptr_t sigmask) {
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_pselect* args = (sargs_SYS_pselect*) edge_syscall->data;

  edge_syscall->syscall_num = SYS_pselect6;
  args->nfds = nfds; 

  if (readfds != 0) {
      args->readfds_is_null = 0; 
      copy_from_user(&args->readfds, (void *) readfds, sizeof(fd_set)); 
  } else {
      args->readfds_is_null = 1; 
  } 
  if (writefds != 0) {
      args->writefds_is_null = 0; 
      copy_from_user(&args->writefds, (void *) writefds, sizeof(fd_set)); 
  } else {
      args->writefds_is_null = 1; 
  } 
  if (exceptfds != 0) {
      args->exceptfds_is_null = 0; 
      copy_from_user(&args->exceptfds, (void *) exceptfds, sizeof(fd_set)); 
  } else {
      args->exceptfds_is_null = 1; 
  } 
  if (timeout != 0) {
      args->timeout_is_null = 0; 
      copy_from_user(&args->timeout, (void *) timeout, sizeof(struct timespec)); 
  } else {
      args->timeout_is_null = 1; 
  } 
  if (sigmask != 0) {
      args->sigmask_is_null = 0; 
      copy_from_user(&args->sigmask, (void *) sigmask, sizeof(sigset_t));  
  } else {
      args->sigmask_is_null = 1; 
  } 

  size_t totalsize = (sizeof(struct edge_syscall)) + sizeof(sargs_SYS_pselect);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  if (ret >= 0) {
      if (readfds != 0) 
          copy_to_user((void *) readfds, &args->readfds, sizeof(fd_set)); 
      if (writefds != 0)
          copy_to_user((void *) writefds, &args->writefds, sizeof(fd_set)); 
      if (exceptfds != 0)
          copy_to_user((void *) exceptfds, &args->exceptfds, sizeof(fd_set));
  }

  print_strace("[runtime] proxied pselect: nfds: %d, ret: %d\r\n", args->nfds, ret);
  return ret;
}

uintptr_t io_syscall_ppoll(uintptr_t fds, int nfds, uintptr_t timeout, uintptr_t sigmask) {
  uintptr_t ret = -1;

  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_ppoll* args = (sargs_SYS_ppoll*) edge_syscall->data;
  struct pollfd *args_pollfds = (void *) args + sizeof(sargs_SYS_ppoll);

  edge_syscall->syscall_num = SYS_ppoll;
  args->nfds = nfds;

  if(timeout != 0) {
      args->timeout_is_null = 0;
      copy_from_user(&args->timeout_ts, (void *) timeout, sizeof(struct timespec));
  } else {
      args->timeout_is_null = 1;
  }

  if(sigmask != 0) {
      args->sigmask_is_null = 0;
      copy_from_user(&args->sigmask, (void *) sigmask, sizeof(sigset_t));
  } else {
      args->sigmask_is_null = 1;
  }

  if(nfds != 0) {
      if(edge_call_check_ptr_valid((uintptr_t) args_pollfds,
                                    nfds * sizeof(struct pollfd)) != 0) {
          goto done;
      }
      copy_from_user(args_pollfds, (void *) fds,nfds * sizeof(struct pollfd));
  }

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_ppoll) +
                        nfds * sizeof(struct pollfd);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  // Copy pollfds back to the eapp
  if(nfds != 0) {
      copy_to_user((void *) fds, args_pollfds, nfds * sizeof(struct pollfd));
  }

done:
  print_strace("[runtime] proxied ppoll: %d\r\n", ret);
  return ret;
}

uintptr_t io_syscall_sendmsg(int sockfd, uintptr_t msg, int flags) {
  uintptr_t ret = -1;

  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  edge_syscall->syscall_num = SYS_sendmsg;

  sargs_SYS_sendrecvmsg *args = (sargs_SYS_sendrecvmsg *) edge_syscall->data;

  args->sockfd = sockfd;
  args->flags = flags;

  // todo not done

//
//  if(edge_call_check_ptr_valid((uintptr_t)&args->msghdr, sizeof(struct msghdr)) != 0){
//      goto done;
//  }
//
//  copy_from_user(&args->msghdr, (void *) msg, sizeof(struct msghdr));

  size_t totalsize = sizeof(struct edge_syscall) + sizeof(sargs_SYS_sendrecvmsg);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

//done:
  print_strace("[runtime] proxied sendmsg\n: %d", ret);
  return ret;
}

uintptr_t io_syscall_recvmsg(int sockfd, uintptr_t msg, int flags) {
  int i;
  struct msghdr msghdr;
  struct iovec *iovec;
  void *curr, *iovec_data;
  uintptr_t ret = -1;

  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  edge_syscall->syscall_num = SYS_recvmsg;

  sargs_SYS_sendrecvmsg *args = (sargs_SYS_sendrecvmsg *) edge_syscall->data;

  args->sockfd = sockfd;
  args->flags = flags;
  args->msg_namelen = args->msg_name_offs =
      args->msg_iovlen = args->msg_iov_offs =
          args->msg_controllen = args->msg_control_offs = 0;

  copy_from_user(&msghdr, (void *) msg, sizeof(struct msghdr));
  curr = (void *) args + sizeof(struct sargs_SYS_sendrecvmsg);

  if(msghdr.msg_namelen != 0) {
      if(edge_call_check_ptr_valid((uintptr_t) curr, msghdr.msg_namelen) != 0){
          goto done;
      }

      copy_from_user(curr, msghdr.msg_name, msghdr.msg_namelen);
      args->msg_namelen = msghdr.msg_namelen;
      args->msg_name_offs = (curr - (void *) args);
      curr += args->msg_namelen;
  }

  if(msghdr.msg_iovlen != 0) {
      // First, copy the iovec vector directly
      if(edge_call_check_ptr_valid((uintptr_t) curr, msghdr.msg_iovlen * sizeof(struct iovec)) != 0){
          goto done;
      }

      copy_from_user(curr, msghdr.msg_iov, msghdr.msg_iovlen * sizeof(struct iovec));
      args->msg_iovlen = msghdr.msg_iovlen;
      args->msg_iov_offs = (curr - (void *) args);

      // Save iovec info for later
      iovec = curr;
      curr += msghdr.msg_iovlen * sizeof(struct iovec);
      iovec_data = curr;

      // We don't need to copy each vector for a receive operation, but we do
      // need to reserve buffer space for every entry
      for(i = 0; i < msghdr.msg_iovlen; i++) {
          if(edge_call_check_ptr_valid((uintptr_t) curr, iovec[i].iov_len) != 0){
            goto done;
          }

//          copy_from_user(curr, iovec[i].iov_base, iovec[i].iov_len);
          curr += iovec[i].iov_len;
      }
  }

  if(msghdr.msg_controllen != 0) {
      if(edge_call_check_ptr_valid((uintptr_t) curr, msghdr.msg_controllen) != 0){
          goto done;
      }

      copy_from_user(curr, msghdr.msg_control, msghdr.msg_controllen);
      args->msg_controllen = msghdr.msg_controllen;
      args->msg_control_offs = (curr - (void *) args);
      curr += args->msg_namelen;
  }

  size_t totalsize = sizeof(struct edge_syscall) + (curr - (void *) args);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

  // Copy flags back
  msghdr.msg_flags = args->msg_flags;
  copy_to_user((void *) msg, &msghdr, sizeof(struct msghdr));

  // Copy back the iovec array, since the host may have trashed this
  copy_from_user(iovec, msghdr.msg_iov, msghdr.msg_iovlen * sizeof(struct iovec));
  if(msghdr.msg_iovlen != 0) {
      for(i = 0; i < msghdr.msg_iovlen; i++) {
          copy_to_user(iovec[i].iov_base, iovec_data, iovec[i].iov_len);
          iovec_data += iovec[i].iov_len;
      }
  }

done:
  print_strace("[runtime] proxied recvmsg: %d\n", ret);
  return ret;
}

uintptr_t io_syscall_connect(int sockfd, uintptr_t addr, socklen_t addrlen) {
  uintptr_t ret = -1;
  struct edge_syscall* edge_syscall = (struct edge_syscall*)edge_call_data_ptr();
  sargs_SYS_connect* args = (sargs_SYS_connect*) edge_syscall->data;

  edge_syscall->syscall_num = SYS_connect;
  args->sockfd = sockfd;
  args->addrlen = addrlen;

  if(edge_call_check_ptr_valid((uintptr_t) &args->addr, args->addrlen) != 0) {
    goto done;
  }

  copy_from_user(&args->addr, (void *) addr, args->addrlen);
  size_t totalsize = (sizeof(struct edge_syscall)) + sizeof(sargs_SYS_connect);
  ret = dispatch_edgecall_syscall(edge_syscall, totalsize);

done:
  print_strace("[runtime] proxied connect: fd: %d, ret: %d\r\n", args->sockfd, ret);
  return ret;
}


#endif /* USE_NET_SYSCALL */
