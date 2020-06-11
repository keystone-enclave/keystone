#if __riscv_xlen == 64
# define STORE    sd
# define LOAD     ld
# define LOG_REGBYTES 3
#elif __riscv_xlen == 32
# define STORE    sw
# define LOAD     lw
# define LOG_REGBYTES 2
#endif

#define REGBYTES (1 << LOG_REGBYTES)

.text

#define TRY(tryname, r_mtvec, r_mepc, r_mcause, r_mstatus) \
    la r_mtvec, tryname##_err     ;\
    csrrw r_mtvec, mtvec, r_mtvec ;\
    csrr r_mepc, mepc             ;\
    csrr r_mcause, mcause         ;\
    csrr r_mstatus, mstatus

#define CATCH_ERR(tryname, r_mepc) \
        j tryname##_ok    ;\
        .align 4          ;\
    tryname##_err :       ;\
        la r_mepc, tryname##_err_real ;\
        csrw mepc, r_mepc ;\
        mret              ;\
        .align 4          ;\
    tryname##_err_real :

#define TRY_END(tryname, r_mtvec, r_mepc, r_mcause, r_mstatus) \
    tryname##_ok :             ;\
        csrw mtvec, r_mtvec    ;\
        csrw mepc, r_mepc      ;\
        csrw mcause, r_mcause  ;\
        csrw mstatus, r_mstatus


#define SET_MPRV(reg)  \
    li reg, 1         ;\
    slli reg, reg, 17 ;\
    csrs mstatus, reg

#define UNSET_MPRV(reg) \
    li reg, 1          ;\
    slli reg, reg, 17  ;\
    csrc mstatus, reg



.global copy_block_to_sm
copy_block_to_sm:
    # Arguments:
    # a0: dst
    # a1: src
    # Temporaries
    # a2: mtvec
    # a3: mepc
    # a4: mcause
    # a5: mstatus
TRY(copy_block_in_check, a2, a3, a4, a5)
    SET_MPRV(t0)
    LOAD t0, 0*REGBYTES(a1)
    LOAD t1, 1*REGBYTES(a1)
    LOAD t2, 2*REGBYTES(a1)
    LOAD t3, 3*REGBYTES(a1)
    LOAD t4, 4*REGBYTES(a1)
    LOAD t5, 5*REGBYTES(a1)
    LOAD t6, 6*REGBYTES(a1)
    LOAD a6, 7*REGBYTES(a1)
    UNSET_MPRV(a1)
    STORE t0, 0*REGBYTES(a0)
    STORE t1, 1*REGBYTES(a0)
    STORE t2, 2*REGBYTES(a0)
    STORE t3, 3*REGBYTES(a0)
    STORE t4, 4*REGBYTES(a0)
    STORE t5, 5*REGBYTES(a0)
    STORE t6, 6*REGBYTES(a0)
    STORE a6, 7*REGBYTES(a0)
    li a0, 0
CATCH_ERR(copy_block_in_check, a1)
    li a0, -1
TRY_END(copy_block_in_check, a2, a3, a4, a5)

    UNSET_MPRV(t0)
    ret


.global copy_word_to_sm
copy_word_to_sm:
    # a0: dst
    # a1: src
TRY(copy_word_in_check, t1, t2, t3, t4)
    SET_MPRV(t5)
    LOAD t0, 0x00(a1)
    UNSET_MPRV(t5)
    STORE t0, 0x00(a0)
    li a0, 0
CATCH_ERR(copy_word_in_check, t5)
    li a0, -1
TRY_END(copy_word_in_check, t1, t2, t3, t4)

    UNSET_MPRV(t5)
    ret


.global copy1_to_sm
copy1_to_sm:
    # a0: dst
    # a1: src
TRY(copy1in_check, t1, t2, t3, t4)
    SET_MPRV(t5)
    lb t0, 0x00(a1)
    UNSET_MPRV(t5)
    sb t0, 0x00(a0)
    li a0, 0
CATCH_ERR(copy1in_check, t5)
    li a0, -1
TRY_END(copy1in_check, t1, t2, t3, t4)

    UNSET_MPRV(t5)
    ret


.global copy_block_from_sm
copy_block_from_sm:
    # Arguments:
    # a0: dst
    # a1: src
    # Temporaries
    # a2: mtvec
    # a3: mepc
    # a4: mcause
    # a5: mstatus
TRY(copy_block_out_check, a2, a3, a4, a5)
    LOAD t0, 0*REGBYTES(a1)
    LOAD t1, 1*REGBYTES(a1)
    LOAD t2, 2*REGBYTES(a1)
    LOAD t3, 3*REGBYTES(a1)
    LOAD t4, 4*REGBYTES(a1)
    LOAD t5, 5*REGBYTES(a1)
    LOAD t6, 6*REGBYTES(a1)
    LOAD a6, 7*REGBYTES(a1)
    SET_MPRV(a1)
    STORE t0, 0*REGBYTES(a0)
    STORE t1, 1*REGBYTES(a0)
    STORE t2, 2*REGBYTES(a0)
    STORE t3, 3*REGBYTES(a0)
    STORE t4, 4*REGBYTES(a0)
    STORE t5, 5*REGBYTES(a0)
    STORE t6, 6*REGBYTES(a0)
    STORE a6, 7*REGBYTES(a0)
    UNSET_MPRV(a1)
    li a0, 0
CATCH_ERR(copy_block_out_check, a1)
    li a0, -1
TRY_END(copy_block_out_check, a2, a3, a4, a5)

    UNSET_MPRV(a1)
    ret


.global copy_word_from_sm
copy_word_from_sm:
    # a0: dst
    # a1: src
TRY(copy_word_out_check, t1, t2, t3, t4)
    LOAD t0, 0x00(a1)
    SET_MPRV(t5)
    STORE t0, 0x00(a0)
    li a0, 0
CATCH_ERR(copy_word_out_check, t5)
    li a0, -1
TRY_END(copy_word_out_check, t1, t2, t3, t4)

    UNSET_MPRV(t5)
    ret


.global copy1_from_sm
copy1_from_sm:
    # a0: dst
    # a1: src
TRY(copy1out_check, t1, t2, t3, t4)
    lb t0, 0x00(a1)
    SET_MPRV(t5)
    sb t0, 0x00(a0)
    li a0, 0
CATCH_ERR(copy1out_check, t5)
    li a0, -1
TRY_END(copy1out_check, t1, t2, t3, t4)

    UNSET_MPRV(t5)
    ret

