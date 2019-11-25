#define STACK_TOP     0x84000000
#define STACK_BOTTOM  0x83c00000
#define DRAM_BASE     0x80000000
#define DTB   0
#define INIT  8

reset:
  li sp, STACK_TOP
  addi sp, sp, -16

  sd x0, INIT(sp)

  csrr t0, mhartid
  bnez t0, loop
  
  # sd a1, DTB(sp)
  
  /* secure boot */
  call bootloader

  /* clear_stack_from_bottom */
  li t1, STACK_BOTTOM
clean_loop:
  beq t1, sp, clean_done
  sd x0, (t1)
  addi t1, t1, 8
  j clean_loop
clean_done:
  li t0, 1
  sd t0, INIT(sp)
  fence

boot:
  call clear_all_but_sp
  
  # boot payload
	csrr a0, mhartid
  la a1, _dtb
	# ld a1, DTB(sp)
  li t0, DRAM_BASE
  jr t0

loop:
  ld t1, INIT(sp)
  beqz t1, loop
  j boot

clear_all_but_sp:
  // mv ra, x0 // also do not clear ra; we should return
  mv gp, x0
  mv tp, x0
  mv t0, x0
  mv t1, x0
  mv t2, x0
  mv s0, x0
  mv s1, x0
  mv a0, x0
  mv a1, x0
  mv a2, x0
  mv a3, x0
  mv a4, x0
  mv a5, x0
  mv a6, x0
  mv a7, x0
  mv s2, x0
  mv s3, x0
  mv s4, x0
  mv s5, x0
  mv s6, x0
  mv s7, x0
  mv s8, x0
  mv s9, x0
  mv s10, x0
  mv s11, x0
  mv t3, x0
  mv t4, x0
  mv t5, x0
  mv t6, x0
  jr ra
