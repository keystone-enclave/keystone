.section .text

eapp_entry:
  li a0, 10000000
  mv a1, zero
loop:
  beq a0, a1, return
  addi a0, a0, -1
  j loop
return:
  li a0, 54321
  li a7, 1101 
  ecall
