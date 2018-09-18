#include "regs.h"

.text
_start:
//entry point to the runtime!
trampoline:
  //keep original stack pointer for the OS
  mv t0, sp
  // runtime stack starts from 0 and grows downward
  li sp, 0

  addi sp, sp, -REGBYTES
  STORE t0, (sp)

  csrw sepc, a0
  csrrw sp, sscratch, sp // TODO: user stack is not stored in sscratch at this point
  sret

encl_trap_handler:
  csrrw sp, sscratch, sp
  addi sp, sp, -CONTEXT_SIZE

  STORE a0, 10*REGBYTES(sp)
  STORE a1, 11*REGBYTES(sp)

  csrr a1, scause
  bge a1, zero, 1f

  /* handle interrupts */
  j not_implemented
1:
  /* handle syscalls */
  # Preserve the registers.  Compute the address of the trap handler.
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

1:auipc t0, %pcrel_hi(rt_trap_table)  # t0 <- %hi(trap_table)
  sll t1, a1, LOG_REGBYTES                    # t1 <- scause << 2 or 3
  add t1, t0, t1                   # t1 <- %hi(trap_table)[scause]
  LWU t1, %pcrel_lo(1b)(t1)         # t1 <- trap_table[scause]
#  csrr a2, sepc                    # a2 <- mepc
#  mv a0, sp                        # a0 <- regs

1:auipc t0, %pcrel_hi(_start)
  addi t0, t0, %pcrel_lo(1b)
  add t1, t1, t0
  jalr t1
  
exit_enclave:
  # TODO: clear all the registers except sp
  LOAD a0, 10*REGBYTES(sp) # restore return
  li t0, CONTEXT_SIZE
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
