#ifdef IO_NET_SYSCALL_WRAPPING
#ifndef _IO_NET_WRAP_H_
#define _IO_NET_WRAP_H_

#include <sys/uio.h>
#include <sys/types.h>
#include "edge_syscall.h"

uintptr_t io_syscall_socket(int domain, int type, int protocol);
uintptr_t io_syscall_setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len); 
uintptr_t io_syscall_bind (int sockfd, uintptr_t addr, socklen_t addrlen);
uintptr_t io_syscall_listen(int sockfd, int backlog);
uintptr_t io_syscall_getpeername(int sockfd, uintptr_t addr, uintptr_t addrlen);
uintptr_t io_syscall_accept(int sockfd, uintptr_t addr, uintptr_t addrlen);

#endif /* _IO_NET_WRAP_H_ */
#endif /* IO_NET_SYSCALL_WRAPPING */