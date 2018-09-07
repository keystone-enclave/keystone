trampoline:
  csrw sepc, a0
  la ra, 1f
  sret
1:
  // a0 = result
  li a7, 1101
  ecall

encl_trap_handler:
  //SAVE ALL (TODO)
  //csrw sscratch, x0
  la ra, ret_from_trap
  csrr s4, scause
  bge s4, zero, 1f

  /* handle interrupts */
  //TODO
  j not_implemented

1:
  /* handle syscalls */
  li t0, 8 //Environment Call
  beq s4, t0, trap_ecall
  j not_implemented

ret_from_trap:

trap_ecall:
  li a7, 1101
  ecall

not_implemented:
  li a7, 1111
  ecall
  
  .section ".rodata"
excp_vect_table:
  .dword not_implemented //0
  .dword not_implemented //1
  .dword not_implemented //2
  .dword not_implemented //3
  .dword not_implemented //4
  .dword not_implemented //5
  .dword not_implemented //6
  .dword not_implemented //7 
  .dword trap_ecall //8
  .dword not_implemented //9
  .dword not_implemented //10
  .dword not_implemented //11
  .dword not_implemented //12
  .dword not_implemented //13
  .dword not_implemented //14
  .dword not_implemented //15
