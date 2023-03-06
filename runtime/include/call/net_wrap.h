#ifdef USE_NET_SYSCALL
#ifndef _NET_WRAP_H_
#define _NET_WRAP_H_

#include <sys/uio.h>
#include <sys/types.h>
#include <sys/select.h>
#include "edge_syscall.h"

uintptr_t io_syscall_socket(int domain, int type, int protocol);
uintptr_t io_syscall_setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len); 
uintptr_t io_syscall_bind (int sockfd, uintptr_t addr, socklen_t addrlen);
uintptr_t io_syscall_listen(int sockfd, int backlog);
uintptr_t io_syscall_getpeername(int sockfd, uintptr_t addr, uintptr_t addrlen);
uintptr_t io_syscall_getsockname(int sockfd, uintptr_t addr, uintptr_t addrlen);
uintptr_t io_syscall_accept(int sockfd, uintptr_t addr, uintptr_t addrlen);
uintptr_t io_syscall_recvfrom(int sockfd, uintptr_t buf, size_t len, int flags,
                				uintptr_t src_addr, uintptr_t addrlen);
uintptr_t io_syscall_sendto(int sockfd, uintptr_t buf, size_t len, int flags,
                				uintptr_t dest_addr, int addrlen);
uintptr_t io_syscall_sendfile(int out_fd, int in_fd, uintptr_t offset, int count);
uintptr_t io_syscall_getuid();
uintptr_t io_syscall_pselect(int nfds, uintptr_t readfds, uintptr_t writefds, uintptr_t exceptfds, uintptr_t timeout, uintptr_t sigmask);
#endif /* _NET_WRAP_H_ */
#endif /* USE_NET_SYSCALL */
