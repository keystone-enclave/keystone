#include <sbi/sbi_trap.h>
#include <sbi/riscv_encoding.h>
#include <sbi/riscv_asm.h>
#include <sbi/sbi_scratch.h>
  .option norvc
  .section .text.init,"ax",@progbits
  .globl trap_vector_enclave

trap_vector_enclave:
    /* Swap TP and MSCRATCH */
    csrrw    tp, CSR_MSCRATCH, tp

    /* Save T0 in scratch space */
    REG_S    t0, SBI_SCRATCH_TMP0_OFFSET(tp)

    /*
     * Set T0 to appropriate exception stack
     *
     * Came_From_M_Mode = ((MSTATUS.MPP < PRV_M) ? 1 : 0) - 1;
     * Exception_Stack = TP ^ (Came_From_M_Mode & (SP ^ TP))
     *
     * Came_From_M_Mode = 0    ==>    Exception_Stack = TP
     * Came_From_M_Mode = -1   ==>    Exception_Stack = SP
     */
    csrr    t0, CSR_MSTATUS
    srl    t0, t0, MSTATUS_MPP_SHIFT
    and    t0, t0, PRV_M
    slti    t0, t0, PRV_M
    add    t0, t0, -1
    xor    sp, sp, tp
    and    t0, t0, sp
    xor    sp, sp, tp
    xor    t0, tp, t0

    /* Save original SP on exception stack */
    REG_S    sp, (SBI_TRAP_REGS_OFFSET(sp) - SBI_TRAP_REGS_SIZE)(t0)

    /* Set SP to exception stack and make room for trap registers */
    add    sp, t0, -(SBI_TRAP_REGS_SIZE)

    /* Restore T0 from scratch space */
    REG_L    t0, SBI_SCRATCH_TMP0_OFFSET(tp)

    /* Save T0 on stack */
    REG_S    t0, SBI_TRAP_REGS_OFFSET(t0)(sp)

    /* Swap TP and MSCRATCH */
    csrrw    tp, CSR_MSCRATCH, tp

    /* Save MEPC and MSTATUS CSRs */
    csrr    t0, CSR_MEPC
    REG_S    t0, SBI_TRAP_REGS_OFFSET(mepc)(sp)
    csrr    t0, CSR_MSTATUS
    REG_S    t0, SBI_TRAP_REGS_OFFSET(mstatus)(sp)
    REG_S    zero, SBI_TRAP_REGS_OFFSET(mstatusH)(sp)
#if __riscv_xlen == 32
    csrr    t0, CSR_MISA
    srli    t0, t0, ('H' - 'A')
    andi    t0, t0, 0x1
    beq    t0, zero, _skip_mstatush_save
    csrr    t0, CSR_MSTATUSH
    REG_S    t0, SBI_TRAP_REGS_OFFSET(mstatusH)(sp)
_skip_mstatush_save:
#endif

    /* Save all general regisers except SP and T0 */
    REG_S    zero, SBI_TRAP_REGS_OFFSET(zero)(sp)
    REG_S    ra, SBI_TRAP_REGS_OFFSET(ra)(sp)
    REG_S    gp, SBI_TRAP_REGS_OFFSET(gp)(sp)
    REG_S    tp, SBI_TRAP_REGS_OFFSET(tp)(sp)
    REG_S    t1, SBI_TRAP_REGS_OFFSET(t1)(sp)
    REG_S    t2, SBI_TRAP_REGS_OFFSET(t2)(sp)
    REG_S    s0, SBI_TRAP_REGS_OFFSET(s0)(sp)
    REG_S    s1, SBI_TRAP_REGS_OFFSET(s1)(sp)
    REG_S    a0, SBI_TRAP_REGS_OFFSET(a0)(sp)
    REG_S    a1, SBI_TRAP_REGS_OFFSET(a1)(sp)
    REG_S    a2, SBI_TRAP_REGS_OFFSET(a2)(sp)
    REG_S    a3, SBI_TRAP_REGS_OFFSET(a3)(sp)
    REG_S    a4, SBI_TRAP_REGS_OFFSET(a4)(sp)
    REG_S    a5, SBI_TRAP_REGS_OFFSET(a5)(sp)
    REG_S    a6, SBI_TRAP_REGS_OFFSET(a6)(sp)
    REG_S    a7, SBI_TRAP_REGS_OFFSET(a7)(sp)
    REG_S    s2, SBI_TRAP_REGS_OFFSET(s2)(sp)
    REG_S    s3, SBI_TRAP_REGS_OFFSET(s3)(sp)
    REG_S    s4, SBI_TRAP_REGS_OFFSET(s4)(sp)
    REG_S    s5, SBI_TRAP_REGS_OFFSET(s5)(sp)
    REG_S    s6, SBI_TRAP_REGS_OFFSET(s6)(sp)
    REG_S    s7, SBI_TRAP_REGS_OFFSET(s7)(sp)
    REG_S    s8, SBI_TRAP_REGS_OFFSET(s8)(sp)
    REG_S    s9, SBI_TRAP_REGS_OFFSET(s9)(sp)
    REG_S    s10, SBI_TRAP_REGS_OFFSET(s10)(sp)
    REG_S    s11, SBI_TRAP_REGS_OFFSET(s11)(sp)
    REG_S    t3, SBI_TRAP_REGS_OFFSET(t3)(sp)
    REG_S    t4, SBI_TRAP_REGS_OFFSET(t4)(sp)
    REG_S    t5, SBI_TRAP_REGS_OFFSET(t5)(sp)
    REG_S    t6, SBI_TRAP_REGS_OFFSET(t6)(sp)

    /* Call C routine */
    add    a0, sp, zero
    call    sbi_trap_handler_keystone_enclave

    /* Restore all general regisers except SP and T0 */
    REG_L    ra, SBI_TRAP_REGS_OFFSET(ra)(sp)
    REG_L    gp, SBI_TRAP_REGS_OFFSET(gp)(sp)
    REG_L    tp, SBI_TRAP_REGS_OFFSET(tp)(sp)
    REG_L    t1, SBI_TRAP_REGS_OFFSET(t1)(sp)
    REG_L    t2, SBI_TRAP_REGS_OFFSET(t2)(sp)
    REG_L    s0, SBI_TRAP_REGS_OFFSET(s0)(sp)
    REG_L    s1, SBI_TRAP_REGS_OFFSET(s1)(sp)
    REG_L    a0, SBI_TRAP_REGS_OFFSET(a0)(sp)
    REG_L    a1, SBI_TRAP_REGS_OFFSET(a1)(sp)
    REG_L    a2, SBI_TRAP_REGS_OFFSET(a2)(sp)
    REG_L    a3, SBI_TRAP_REGS_OFFSET(a3)(sp)
    REG_L    a4, SBI_TRAP_REGS_OFFSET(a4)(sp)
    REG_L    a5, SBI_TRAP_REGS_OFFSET(a5)(sp)
    REG_L    a6, SBI_TRAP_REGS_OFFSET(a6)(sp)
    REG_L    a7, SBI_TRAP_REGS_OFFSET(a7)(sp)
    REG_L    s2, SBI_TRAP_REGS_OFFSET(s2)(sp)
    REG_L    s3, SBI_TRAP_REGS_OFFSET(s3)(sp)
    REG_L    s4, SBI_TRAP_REGS_OFFSET(s4)(sp)
    REG_L    s5, SBI_TRAP_REGS_OFFSET(s5)(sp)
    REG_L    s6, SBI_TRAP_REGS_OFFSET(s6)(sp)
    REG_L    s7, SBI_TRAP_REGS_OFFSET(s7)(sp)
    REG_L    s8, SBI_TRAP_REGS_OFFSET(s8)(sp)
    REG_L    s9, SBI_TRAP_REGS_OFFSET(s9)(sp)
    REG_L    s10, SBI_TRAP_REGS_OFFSET(s10)(sp)
    REG_L    s11, SBI_TRAP_REGS_OFFSET(s11)(sp)
    REG_L    t3, SBI_TRAP_REGS_OFFSET(t3)(sp)
    REG_L    t4, SBI_TRAP_REGS_OFFSET(t4)(sp)
    REG_L    t5, SBI_TRAP_REGS_OFFSET(t5)(sp)
    REG_L    t6, SBI_TRAP_REGS_OFFSET(t6)(sp)

    /* Restore MEPC and MSTATUS CSRs */
    REG_L    t0, SBI_TRAP_REGS_OFFSET(mepc)(sp)
    csrw    CSR_MEPC, t0
    REG_L    t0, SBI_TRAP_REGS_OFFSET(mstatus)(sp)
    csrw    CSR_MSTATUS, t0
#if __riscv_xlen == 32
    csrr    t0, CSR_MISA
    srli    t0, t0, ('H' - 'A')
    andi    t0, t0, 0x1
    beq    t0, zero, _skip_mstatush_restore
    REG_L    t0, SBI_TRAP_REGS_OFFSET(mstatusH)(sp)
    csrw    CSR_MSTATUSH, t0
_skip_mstatush_restore:
#endif

    /* Restore T0 */
    REG_L    t0, SBI_TRAP_REGS_OFFSET(t0)(sp)

    /* Restore SP */
    REG_L    sp, SBI_TRAP_REGS_OFFSET(sp)(sp)

    mret
