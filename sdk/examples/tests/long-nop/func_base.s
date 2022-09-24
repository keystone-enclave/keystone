addi sp, sp, -64
  li a0, 0xdeadbeef
  STORE a0, (sp)
return:
  li a0, 12345
  li a7, 1101 
  ecall
