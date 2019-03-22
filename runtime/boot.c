#include <stdint.h>
#include <stddef.h>
#include <asm/csr.h>

#include "printf.h"
#include "interrupt.h"
#include "syscall.h"
#include "vm.h"
/* defined in syscall.c */
extern uintptr_t shared_buffer;
extern uintptr_t shared_buffer_size;

/* initial memory layout */
uintptr_t freemem_base;
size_t freemem_size;
uintptr_t utm_base;
size_t utm_size;

/* defined in entry.S */
extern void* encl_trap_handler;

void
init_freemem(uintptr_t freemem_base,
             size_t freemem_size)
{
  /* initialize free memory with a simple page allocator*/

}

void remap_kernel_space(uintptr_t dram_base,
                        uintptr_t dram_size)
{
}

void
eyrie_boot(uintptr_t dummy, // $a0 contains the return value from the SBI
           uintptr_t dram_base,
           uintptr_t dram_size,
           uintptr_t runtime_paddr,
           uintptr_t user_paddr,
           uintptr_t free_paddr,
           uintptr_t utm_vaddr,
           uintptr_t utm_size)
{
  /* set initial values */
  freemem_base = free_paddr;
  freemem_size = dram_base + dram_size - free_paddr;
  shared_buffer = utm_vaddr;
  shared_buffer_size = utm_size;

  /* set trap vector */
  csr_write(stvec, &encl_trap_handler);

  /* set timer */
  init_timer();

  /* prepare edge & system calls */
  init_edge_internals();
}
