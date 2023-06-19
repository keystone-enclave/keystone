#if __riscv_xlen == 64
# define STORE    sd
# define LOAD     ld
# define LOG_REGBYTES 3
#elif __riscv_xlen == 32
# define STORE    sw
# define LOAD     lw
# define LOG_REGBYTES 2
#endif

.section .text

eapp_entry:
add:
  addi sp, sp, -64
  li a0, 0xdeadbeef
  STORE a0, (sp)
return:
  li a0, 12345
  li a7, 1101 
  ecall
