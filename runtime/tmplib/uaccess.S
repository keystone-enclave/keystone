#include <linux/linkage.h>
#include <asm/asm.h>
#include <asm/csr.h>

	.altmacro
	.macro fixup op reg addr lbl
	LOCAL _epc
_epc:
	\op \reg, \addr
	.section __ex_table,"a"
	.balign RISCV_SZPTR
	RISCV_PTR _epc, \lbl
	.previous
	.endm

ENTRY(__asm_copy_to_user)
ENTRY(__asm_copy_from_user)

#define COPY_USER_UNROLL 8
#define COPY_USER_BLKSZ (COPY_USER_UNROLL * SZREG)

#if (COPY_USER_UNROLL) & (COPY_USER_UNROLL - 1) != 0
#error COPY_USER_UNROLL must be a power of 2
#endif

	/* Enable access to user memory */
	li t6, SR_SUM
	csrs sstatus, t6

	/*
	 * a3: terminal address of destination region
	 * t1: lowest XLEN-aligned address in destination
	 * t0: highest XLEN-aligned address in destination for which
	 *     (t0 - t1) is a multiple of the block size
	 */
	addi t1, a0, SZREG-1
	add a3, a0, a2
	andi t1, t1, ~(SZREG-1)
	bgeu t1, a3, 4f /* Skip word copy */

	sub t0, a3, t1
	andi t0, t0, ~((COPY_USER_BLKSZ)-1)
	beqz t0, 4f	/* Skip word copy if smaller than block size */
	add t0, t1, t0

	bltu a0, t1, 6f	/* Handle initial destination misalignment */
1:
	/* Check source alignment */
	/* NOTE: Destination (a0) is now XLEN-aligned */
	andi t1, a1, SZREG-1
	bnez t1, 7f
2:
	.macro fixup_aligned op reg addr lbl n
	.if \n < (COPY_USER_UNROLL)
	fixup \op, \reg, ((\n)*SZREG)\addr, \lbl
	.endif
	.endm

	/* Aligned word-oriented copy */
	fixup_aligned REG_L, t2, (a1), 10f, 0
	fixup_aligned REG_L, t3, (a1), 10f, 1
	fixup_aligned REG_L, t4, (a1), 10f, 2
	fixup_aligned REG_L, t5, (a1), 10f, 3
	fixup_aligned REG_S, t2, (a0), 10f, 0
	fixup_aligned REG_S, t3, (a0), 10f, 1
	fixup_aligned REG_S, t4, (a0), 10f, 2
	fixup_aligned REG_S, t5, (a0), 10f, 3

	fixup_aligned REG_L, t2, (a1), 10f, 4
	fixup_aligned REG_L, t3, (a1), 10f, 5
	fixup_aligned REG_L, t4, (a1), 10f, 6
	fixup_aligned REG_L, t5, (a1), 10f, 7
	fixup_aligned REG_S, t2, (a0), 10f, 4
	fixup_aligned REG_S, t3, (a0), 10f, 5
	fixup_aligned REG_S, t4, (a0), 10f, 6
	fixup_aligned REG_S, t5, (a0), 10f, 7

	addi a0, a0, COPY_USER_BLKSZ
	addi a1, a1, COPY_USER_BLKSZ
	bltu a0, t0, 2b

	bgeu a0, a3, 5f
3:
	/* Edge case: remainder */
	fixup lbu, t2, (a1), 10f
	fixup sb, t2, (a0), 10f
	addi a0, a0, 1
	addi a1, a1, 1
4:
	bltu a0, a3, 3b
5:
	/* Disable access to user memory */
	csrc sstatus, t6
	li a0, 0
	ret
6:
	/* Edge case: initial unalignment */
	fixup lbu, t2, (a1), 10f
	fixup sb, t2, (a0), 10f
	addi a0, a0, 1
	addi a1, a1, 1
	bltu a0, t1, 6b
	j 1b
7:
	/* Unaligned word-oriented copy */
	li t2, SZREG<<3
	slli t1, t1, 3			/* Convert offset to bits */
	andi a1, a1, ~(SZREG-1)
	sub t2, t2, t1
	fixup REG_L, t3, (a1), 10f	/* Read first partial word */
8:
	.macro fixup_unaligned n
	.if \n < (COPY_USER_UNROLL)
	srl t4, t3, t1			/* Extract upper part of previous word */
	fixup REG_L, t3, ((\n+1)*SZREG)(a1), 10f
	sll t5, t3, t2			/* Extract lower part of next word */
	or t4, t4, t5			/* Merge */
	fixup REG_S, t4, (\n*SZREG)(a0), 10f
	.endif
	.endm

	fixup_unaligned 0
	fixup_unaligned 1
	fixup_unaligned 2
	fixup_unaligned 3
	fixup_unaligned 4
	fixup_unaligned 5
	fixup_unaligned 6
	fixup_unaligned 7

	addi a0, a0, COPY_USER_BLKSZ
	addi a1, a1, COPY_USER_BLKSZ
	bltu a0, t0, 8b

	srli t1, t1, 3
	add a1, a1, t1			/* Re-add offset to source pointer */
	j 4b
ENDPROC(__asm_copy_to_user)
ENDPROC(__asm_copy_from_user)


ENTRY(__clear_user)

	/* Enable access to user memory */
	li t6, SR_SUM
	csrs sstatus, t6

	add a3, a0, a1
	addi t0, a0, SZREG-1
	andi t1, a3, ~(SZREG-1)
	andi t0, t0, ~(SZREG-1)
	/*
	 * a3: terminal address of target region
	 * t0: lowest doubleword-aligned address in target region
	 * t1: highest doubleword-aligned address in target region
	 */
	bgeu t0, t1, 2f
	bltu a0, t0, 4f
1:
	fixup REG_S, zero, (a0), 11f
	addi a0, a0, SZREG
	bltu a0, t1, 1b
2:
	bltu a0, a3, 5f

3:
	/* Disable access to user memory */
	csrc sstatus, t6
	li a0, 0
	ret
4: /* Edge case: unalignment */
	fixup sb, zero, (a0), 11f
	addi a0, a0, 1
	bltu a0, t0, 4b
	j 1b
5: /* Edge case: remainder */
	fixup sb, zero, (a0), 11f
	addi a0, a0, 1
	bltu a0, a3, 5b
	j 3b
ENDPROC(__clear_user)

	.section .fixup,"ax"
	.balign 4
	/* Fixup code for __copy_user(10) and __clear_user(11) */
10:
	/* Disable access to user memory */
	csrs sstatus, t6
	mv a0, a2
	ret
11:
	csrs sstatus, t6
	mv a0, a1
	ret
	.previous
