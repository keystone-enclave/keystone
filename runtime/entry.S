#include "regs.h"

#define ENCL_STACK_SIZE (8*1024)

.text
//entry point to the runtime!
_start:  
  j entry
  nop

encl_trap_handler:
  csrrw sp, sscratch, sp
  addi sp, sp, -CONTEXT_SIZE

  STORE a0, 10*REGBYTES(sp)
  STORE a1, 11*REGBYTES(sp)

  csrr a0, scause
  bge a0, zero, 1f 

  /* handle interrupts */
  j not_implemented 
1:
  /* handle syscalls */
  STORE ra, 1*REGBYTES(sp)
  STORE gp, 3*REGBYTES(sp)
  STORE tp, 4*REGBYTES(sp)
  STORE t0, 5*REGBYTES(sp)
  STORE t1, 6*REGBYTES(sp)
  STORE t2, 7*REGBYTES(sp)
  STORE s0, 8*REGBYTES(sp)
  STORE s1, 9*REGBYTES(sp)
  STORE a2,12*REGBYTES(sp)
  STORE a3,13*REGBYTES(sp)
  STORE a4,14*REGBYTES(sp)
  STORE a5,15*REGBYTES(sp)
  STORE a6,16*REGBYTES(sp)
  STORE a7,17*REGBYTES(sp)
  STORE s2,18*REGBYTES(sp)
  STORE s3,19*REGBYTES(sp)
  STORE s4,20*REGBYTES(sp)
  STORE s5,21*REGBYTES(sp)
  STORE s6,22*REGBYTES(sp)
  STORE s7,23*REGBYTES(sp)
  STORE s8,24*REGBYTES(sp)
  STORE s9,25*REGBYTES(sp)
  STORE s10,26*REGBYTES(sp)
  STORE s11,27*REGBYTES(sp)
  STORE t3,28*REGBYTES(sp)
  STORE t4,29*REGBYTES(sp)
  STORE t5,30*REGBYTES(sp)
  STORE t6,31*REGBYTES(sp)
  csrrw t0, sscratch, x0           # t0 <- user sp
  STORE t0, 2*REGBYTES(sp)         # sp 

  la t0, rt_trap_table
  sll t1, a0, LOG_REGBYTES
  add t1, t0, t1
  LOAD t1, 0(t1)

  jalr t1
  
  j not_implemented

entry:
  /* keep the original stack pointer for the OS */
  mv t0, sp
  /* runtime stack starts from 0x0 and grows downward! */
  li sp, 0
  
  addi sp, sp, -2*REGBYTES
  STORE t0, (sp)
  STORE a0, REGBYTES(sp)

  /* TODO: stack size should be flexible */
  li a0, ENCL_STACK_SIZE
  call init_mm

  /* set user stack below the runtime */
  la a0, _start 
  csrw sscratch, a0

trampoline:
  la a0, encl_trap_handler
  csrw stvec, a0
  
  LOAD a0, REGBYTES(sp)
  csrw sepc, a0
  csrrw sp, sscratch, sp
  
  sret

exit_enclave:
  # TODO: clear all the registers except sp
  LOAD a0, 10*REGBYTES(sp) # restore return
  li t0, CONTEXT_SIZE

  # restore the OS sp
  add sp, sp, t0
  LOAD sp, (sp)
  li a7, 1101
  ecall

  
not_implemented:
  li a7, 1111
  ecall

  .section ".rodata"
rt_trap_table:
  .align 6 
  .dword not_implemented //0
  .dword not_implemented //1
  .dword not_implemented //2
  .dword not_implemented //3
  .dword not_implemented //4
  .dword not_implemented //5
  .dword not_implemented //6
  .dword not_implemented //7 
  .dword exit_enclave //8
  .dword not_implemented //9
  .dword not_implemented //10
  .dword not_implemented //11
  .dword not_implemented //12: fetch page fault - code always presents in memory
  .dword rt_page_fault //13: load page fault - stack/heap access
  .dword not_implemented //14
  .dword rt_page_fault //15: store page fault - stack/heap access
