.section .text

add:
  li a0, 0xdeadbeef
  add a0, a0, a0
return:
  li a7, 1101 
  ecall
