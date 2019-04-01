#include "uaccess.h"
#include "linux_wrap.h"
#include "syscall.h"

#define CLOCK_FREQ 1000000000

//TODO we should check which clock this is
uintptr_t linux_clock_gettime(__clockid_t clock, struct timespec *tp){
  print_strace("[runtime] clock_gettime not fully supported (clock %x, assuming)\r\n", clock);
  unsigned long cycles;
  asm volatile ("rdcycle %0" : "=r" (cycles));

  unsigned long sec = cycles / CLOCK_FREQ;
  unsigned long nsec = (cycles % CLOCK_FREQ);

  copy_to_user(&(tp->tv_sec), &sec, sizeof(unsigned long));
  copy_to_user(&(tp->tv_nsec), &nsec, sizeof(unsigned long));

  return 0;
}

uintptr_t linux_set_tid_address(int* tidptr_t){
  //Ignore for now
  print_strace("[runtime] set_tid_address, not setting address (%p), IGNORING\r\n",tidptr_t);
  return 1;
}

uintptr_t linux_rt_sigprocmask(int how, const sigset_t *set, sigset_t *oldset){
  print_strace("[runtime] rt_sigprocmask not supported (how %x), IGNORING\r\n", how);
  return 0;
}

uintptr_t linux_RET_ZERO_wrap(unsigned long which){
  print_strace("[runtime] Cannot handle syscall %lu, IGNORING = 0\r\n", which);
  return 0;
}

uintptr_t linux_RET_BAD_wrap(unsigned long which){
  print_strace("[runtime] Cannot handle syscall %lu, FAILING = -1\r\n", which);
  return -1;
}

uintptr_t linux_getpid(){
  uintptr_t fakepid = 2;
  print_strace("[runtime] Faking getpid with %lx\r\n",fakepid);
  return fakepid;
}

uintptr_t linux_getrandom(void *buf, size_t buflen, unsigned int flags){
  print_strace("[runtime] [UNSAFE] getrandom not supported (size %lx), returning non-random values\r\n", buflen);
  unsigned char v;
  size_t remaining = buflen;

  unsigned char* next_buf = (unsigned char*)buf;
  while(remaining > 0){
    v = remaining%255;
    copy_to_user(next_buf,&v,sizeof(unsigned char));
    remaining--;
    next_buf++;
  }

  return buflen;
}
