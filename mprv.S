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



.global copy64_to_sm
copy64_to_sm:
    # Arguments:
    # a0: dst
    # a1: src
    # Temporaries
    # a2: mtvec
    # a3: mepc
    # a4: mcause
    # a5: mstatus
TRY(copy64in_check, a2, a3, a4, a5)
    SET_MPRV(t0)
    ld t0, 0x00(a1)
    ld t1, 0x08(a1)
    ld t2, 0x10(a1)
    ld t3, 0x18(a1)
    ld t4, 0x20(a1)
    ld t5, 0x28(a1)
    ld t6, 0x30(a1)
    ld a6, 0x38(a1)
    UNSET_MPRV(a1)
    sd t0, 0x00(a0)
    sd t1, 0x08(a0)
    sd t2, 0x10(a0)
    sd t3, 0x18(a0)
    sd t4, 0x20(a0)
    sd t5, 0x28(a0)
    sd t6, 0x30(a0)
    sd a6, 0x38(a0)
    li a0, 0
CATCH_ERR(copy64in_check, a1)
    li a0, -1
TRY_END(copy64in_check, a2, a3, a4, a5)

    UNSET_MPRV(t0)
    ret


.global copy8_to_sm
copy8_to_sm:
    # a0: dst
    # a1: src
TRY(copy8in_check, t1, t2, t3, t4)
    SET_MPRV(t5)
    ld t0, 0x00(a1)
    UNSET_MPRV(t5)
    sd t0, 0x00(a0)
    li a0, 0
CATCH_ERR(copy8in_check, t5)
    li a0, -1
TRY_END(copy8in_check, t1, t2, t3, t4)

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


.global copy64_from_sm
copy64_from_sm:
    # Arguments:
    # a0: dst
    # a1: src
    # Temporaries
    # a2: mtvec
    # a3: mepc
    # a4: mcause
    # a5: mstatus
TRY(copy64out_check, a2, a3, a4, a5)
    ld t0, 0x00(a1)
    ld t1, 0x08(a1)
    ld t2, 0x10(a1)
    ld t3, 0x18(a1)
    ld t4, 0x20(a1)
    ld t5, 0x28(a1)
    ld t6, 0x30(a1)
    ld a6, 0x38(a1)
    SET_MPRV(a1)
    sd t0, 0x00(a0)
    sd t1, 0x08(a0)
    sd t2, 0x10(a0)
    sd t3, 0x18(a0)
    sd t4, 0x20(a0)
    sd t5, 0x28(a0)
    sd t6, 0x30(a0)
    sd a6, 0x38(a0)
    UNSET_MPRV(a1)
    li a0, 0
CATCH_ERR(copy64out_check, a1)
    li a0, -1
TRY_END(copy64out_check, a2, a3, a4, a5)

    UNSET_MPRV(a1)
    ret


.global copy8_from_sm
copy8_from_sm:
    # a0: dst
    # a1: src
TRY(copy8out_check, t1, t2, t3, t4)
    ld t0, 0x00(a1)
    SET_MPRV(t5)
    sd t0, 0x00(a0)
    li a0, 0
CATCH_ERR(copy8out_check, t5)
    li a0, -1
TRY_END(copy8out_check, t1, t2, t3, t4)

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

