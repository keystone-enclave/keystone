#ifdef USE_LINUX_SYSCALL

#define _GNU_SOURCE
#include "call/linux_wrap.h"

#include <signal.h>
#include <sys/mman.h>
#include <sys/utsname.h>
#include <time.h>

#include "mm/freemem.h"
#include "mm/mm.h"
#include "util/rt_util.h"
#include "call/syscall.h"
#include "uaccess.h"

#define CLOCK_FREQ 1000000000

//TODO we should check which clock this is
uintptr_t linux_clock_gettime(__clockid_t clock, struct timespec *tp){
  print_strace("[runtime] clock_gettime not fully supported (clock %x, assuming)\r\n", clock);
  unsigned long cycles;
  __asm__ __volatile__("rdcycle %0" : "=r"(cycles));

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

  uintptr_t ret = rt_util_getrandom(buf, buflen);
  print_strace("[runtime] getrandom IGNORES FLAGS (size %lx), PLATFORM DEPENDENT IF SAFE = ret %lu\r\n", buflen, ret);
  return ret;
}

#define UNAME_SYSNAME "Linux\0"
#define UNAME_NODENAME "Encl\0"
#define UNAME_RELEASE "4.15.0\0"
#define UNAME_VERSION "Eyrie\0"
#define UNAME_MACHINE "NA\0"

uintptr_t linux_uname(void* buf){
  // Here we go

  struct utsname *user_uname = (struct utsname *)buf;
  uintptr_t ret;

  ret = copy_to_user(&user_uname->sysname, UNAME_SYSNAME, sizeof(UNAME_SYSNAME));
  if(ret != 0) goto uname_done;

  ret = copy_to_user(&user_uname->nodename, UNAME_NODENAME, sizeof(UNAME_NODENAME));
  if(ret != 0) goto uname_done;

  ret = copy_to_user(&user_uname->release, UNAME_RELEASE, sizeof(UNAME_RELEASE));
  if(ret != 0) goto uname_done;

  ret = copy_to_user(&user_uname->version, UNAME_VERSION, sizeof(UNAME_VERSION));
  if(ret != 0) goto uname_done;

  ret = copy_to_user(&user_uname->machine, UNAME_MACHINE, sizeof(UNAME_MACHINE));
  if(ret != 0) goto uname_done;



 uname_done:
  print_strace("[runtime] uname = %x\n",ret);
  return ret;
}

uintptr_t syscall_munmap(void *addr, size_t length){
  uintptr_t ret = (uintptr_t)((void*)-1);

  free_pages(vpn((uintptr_t)addr), length/RISCV_PAGE_SIZE);
  ret = 0;
  return ret;
}

uintptr_t syscall_mmap(void *addr, size_t length, int prot, int flags,
                 int fd, __off_t offset){
  uintptr_t ret = (uintptr_t)((void*)-1);

  int pte_flags = PTE_U | PTE_A;

  if(flags != (MAP_ANONYMOUS | MAP_PRIVATE) || fd != -1){
    // we don't support mmaping any other way yet
    goto done;
  }

  // Set flags
  if(prot & PROT_READ)
    pte_flags |= PTE_R;
  if(prot & PROT_WRITE)
    pte_flags |= PTE_W | PTE_D;
  if(prot & PROT_EXEC)
    pte_flags |= PTE_X;



  // Find a continuous VA space that will fit the req. size
  int req_pages = vpn(PAGE_UP(length));

  // Do we have enough available phys pages?
  if( req_pages > spa_available()){
    goto done;
  }

  // Start looking at EYRIE_ANON_REGION_START for VA space
  uintptr_t starting_vpn = vpn(EYRIE_ANON_REGION_START);
  uintptr_t valid_pages;
  while((starting_vpn + req_pages) <= EYRIE_ANON_REGION_END){
    valid_pages = test_va_range(starting_vpn, req_pages);

    if(req_pages == valid_pages){
      // Set a successful value if we allocate
      // TODO free partial allocation on failure
      if(alloc_pages(starting_vpn, req_pages, pte_flags) == req_pages){
        ret = starting_vpn << RISCV_PAGE_BITS;
      }
      break;
    }
    else
      starting_vpn += valid_pages + 1;
  }

 done:
  print_strace("[runtime] [mmap]: addr: 0x%p, length %lu, prot 0x%x, flags 0x%x, fd %i, offset %lu (%li pages %x) = 0x%p\r\n", addr, length, prot, flags, fd, offset, req_pages, pte_flags, ret);

  // If we get here everything went wrong
  return ret;
}


uintptr_t syscall_brk(void* addr){
  // Two possible valid calls to brk we handle:
  // NULL -> give current break
  // ADDR -> give more pages up to ADDR if possible

  uintptr_t req_break = (uintptr_t)addr;

  uintptr_t current_break = get_program_break();
  uintptr_t ret = current_break;
  int req_page_count = 0;

  // Return current break if null or current break
  if( req_break == 0  || req_break <= current_break){
    goto done;
  }

  // Otherwise try to allocate pages

  // Can we allocate enough phys pages?
  req_page_count = (PAGE_UP(req_break) - current_break) / RISCV_PAGE_SIZE;
  if( spa_available() < req_page_count){
    goto done;
  }

  // Allocate pages
  // TODO free pages on failure
  if( alloc_pages(vpn(current_break),
                  req_page_count,
                  PTE_W | PTE_R | PTE_D | PTE_U | PTE_A)
      != req_page_count){
    goto done;
  }

  // Success
  set_program_break(req_break);
  ret = req_break;


 done:
  print_strace("[runtime] brk (0x%p) (req pages %i) = 0x%p\r\n",req_break, req_page_count, ret);
  return ret;

}
#endif /* USE_LINUX_SYSCALL */
