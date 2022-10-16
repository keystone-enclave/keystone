#ifndef __EDGE_SYSCALL_H_
#define __EDGE_SYSCALL_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>
#include "edge_call.h"
#include "edge_common.h"
#include "syscall_nums.h"
#include "sys/epoll.h"

#ifdef __cplusplus
extern "C" {
#endif

// Special call number
#define EDGECALL_SYSCALL MAX_EDGE_CALL + 1

struct edge_syscall {
  size_t syscall_num;
  unsigned char data[];
};

typedef struct sargs_SYS_openat {
  int dirfd;
  int flags;
  int mode;
  char path[];
} sargs_SYS_openat;

// unlinkat uses (most) of the same args
typedef sargs_SYS_openat sargs_SYS_unlinkat;

typedef struct sargs_SYS_write {
  int fd;
  size_t len;
  unsigned char buf[];
} sargs_SYS_write;

// Read uses the same args as write
typedef sargs_SYS_write sargs_SYS_read;

struct _sargs_fd_only {
  int fd;
};

typedef struct _sargs_fd_only sargs_SYS_fsync;
typedef struct _sargs_fd_only sargs_SYS_close;

typedef struct sargs_SYS_lseek {
  int fd;
  off_t offset;
  int whence;
} sargs_SYS_lseek;

typedef struct sargs_SYS_epoll_create1 {
  int size;
} sargs_SYS_epoll_create1;

typedef struct sargs_SYS_socket{
  int domain;
  int type;
  int protocol;
} sargs_SYS_socket;

typedef struct sargs_SYS_setsockopt{
  int socket;
  int level;
  int option_name;
  int option_value;
  socklen_t option_len;
} sargs_SYS_setsockopt;

typedef struct sargs_SYS_bind{
  int sockfd;
  struct sockaddr_storage addr;
  socklen_t addrlen;
} sargs_SYS_bind;

typedef struct sargs_SYS_listen{
  int sockfd;
  int backlog;
} sargs_SYS_listen;

typedef struct sargs_SYS_accept{
  int sockfd;
  struct sockaddr_storage addr;
  socklen_t addrlen;
} sargs_SYS_accept;

typedef struct sargs_SYS_epoll_ctl{
  int epfd;
  int op;
  int fd;
  uintptr_t event;
} sargs_SYS_epoll_ctl;

typedef struct sargs_SYS_fcntl {
  int fd;
  int cmd;
  int has_struct;
  unsigned long arg[];
} sargs_SYS_fcntl;

typedef struct sargs_SYS_getcwd {
  size_t size;
  char buf[];
} sargs_SYS_getcwd;

typedef struct sargs_SYS_chdir{
  char path[0];
} sargs_SYS_chdir;


typedef struct sargs_SYS_epoll_pwait{
  int epfd;
  struct epoll_event events;
  int maxevents;
  int timeout;
} sargs_SYS_epoll_pwait;


typedef struct sargs_SYS_getpeername{
  int sockfd;
  struct sockaddr_storage addr;
  socklen_t addrlen;
} sargs_SYS_getpeername;

typedef struct sargs_SYS_getsockname{
  int sockfd;
  struct sockaddr addr;
  socklen_t addrlen;
} sargs_SYS_getsockname;

typedef struct sargs_SYS_renameat2{
  int olddirfd;
  char oldpath[128];
  int newdirfd;
  char newpath[128];
  unsigned int flags;
} sargs_SYS_renameat2;



typedef struct sargs_SYS_umask{
  mode_t mask;
} sargs_SYS_umask;

typedef struct sargs_SYS_ftruncate {
  int fd;
  off_t offset;
} sargs_SYS_ftruncate;

typedef struct sargs_SYS_fstatat {
  int dirfd;
  int flags;
  struct stat stats;
  char pathname[];
} sargs_SYS_fstatat;

typedef struct sargs_SYS_fstat {
  int fd;
  struct stat stats;
} sargs_SYS_fstat;

typedef struct sargs_SYS_pselect {
  int nfds; 
  int readfds_is_null; 
  int writefds_is_null; 
  int exceptfds_is_null; 
  int timeout_is_null; 
  int sigmask_is_null; 
  fd_set readfds; 
  fd_set writefds; 
  fd_set exceptfds; 
  struct timespec timeout; 
  sigset_t sigmask; 
} sargs_SYS_pselect;

typedef struct sargs_SYS_recvfrom {
  int sockfd; 
  size_t len; 
  int flags; 
  int src_addr_is_null; 
  struct sockaddr src_addr; 
  socklen_t addrlen; 
  char buf[]; 
} sargs_SYS_recvfrom;

typedef struct sargs_SYS_sendto {
  int sockfd; 
  size_t len; 
  int flags; 
  int dest_addr_is_null; 
  struct sockaddr dest_addr; 
  socklen_t addrlen; 
  char buf[]; 
} sargs_SYS_sendto;

typedef struct sargs_SYS_sendfile {
  int out_fd; 
  size_t in_fd; 
  int offset_is_null;
  off_t offset; 
  size_t count;
} sargs_SYS_sendfile;

void
incoming_syscall(struct edge_call* buffer);

#ifdef __cplusplus
}
#endif

#endif /* __EDGE_SYSCALL_H_ */
