#include "call/sbi.h"

#include "mm/vm_defs.h"

#define SBI_CALL(___ext, ___which, ___arg0, ___arg1,             \
                 ___arg2, ___arg3, ___arg4, ___arg5)             \
  ({                                                             \
    register uintptr_t a0 __asm__("a0") = (uintptr_t)(___arg0);  \
    register uintptr_t a1 __asm__("a1") = (uintptr_t)(___arg1);  \
    register uintptr_t a2 __asm__("a2") = (uintptr_t)(___arg2);  \
    register uintptr_t a3 __asm__("a3") = (uintptr_t)(___arg3);  \
    register uintptr_t a4 __asm__("a4") = (uintptr_t)(___arg4);  \
    register uintptr_t a5 __asm__("a5") = (uintptr_t)(___arg5);  \
    register uintptr_t a6 __asm__("a6") = (uintptr_t)(___which); \
    register uintptr_t a7 __asm__("a7") = (uintptr_t)(___ext);   \
    __asm__ volatile("ecall"                                     \
                     : "+r"(a0)                                  \
                     : "r"(a1), "r"(a2), "r"(a3), "r"(a4),       \
                       "r"(a5), "r"(a6), "r"(a7)                 \
                     : "memory");                                \
    a0;                                                          \
  })

/* Lazy implementations until SBI is finalized */
#define SBI_CALL_0(___ext, ___which) SBI_CALL(___ext, ___which, 0, 0, 0, 0, 0, 0)
#define SBI_CALL_1(___ext, ___which, ___arg0) SBI_CALL(___ext, ___which, ___arg0, 0, 0, 0, 0, 0)
#define SBI_CALL_2(___ext, ___which, ___arg0, ___arg1) \
  SBI_CALL(___ext, ___which, ___arg0, ___arg1, 0, 0, 0, 0)
#define SBI_CALL_3(___ext, ___which, ___arg0, ___arg1, ___arg2) \
  SBI_CALL(___ext, ___which, ___arg0, ___arg1, ___arg2, 0, 0, 0)
#define SBI_CALL_4(___ext, ___which, ___arg0, ___arg1, ___arg2, ___arg3) \
  SBI_CALL(___ext, ___which, ___arg0, ___arg1, ___arg2, ___arg3, 0, 0)
#define SBI_CALL_5(___ext, ___which, ___arg0, ___arg1, ___arg2, ___arg3, ___arg4) \
  SBI_CALL(___ext, ___which, ___arg0, ___arg1, ___arg2, ___arg3, __arg4, 0)
#define SBI_CALL_6(___ext, ___which, ___arg0, ___arg1, ___arg2, ___arg3, ___arg4, ___arg5) \
  SBI_CALL(___ext, ___which, ___arg0, ___arg1, ___arg2, ___arg3, ___arg4, ___arg5)

void
sbi_putchar(char character) {
  SBI_CALL_1(SBI_CONSOLE_PUTCHAR, 0, character);
}

void
sbi_set_timer(uint64_t stime_value) {
#if __riscv_xlen == 32
  SBI_CALL_2(SBI_SET_TIMER, 0, stime_value, stime_value >> 32);
#else
  SBI_CALL_1(SBI_SET_TIMER, 0, stime_value);
#endif
}

uintptr_t
sbi_stop_enclave(uint64_t request) {
  return SBI_CALL_1(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_STOP_ENCLAVE, request);
}

void
sbi_exit_enclave(uint64_t retval) {
  SBI_CALL_1(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_EXIT_ENCLAVE, retval);
}

uintptr_t
sbi_random() {
  SBI_CALL_0(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_RANDOM);
  register uintptr_t a1 __asm__("a1");
  return a1;
}

uintptr_t
sbi_query_multimem(size_t *size) {
  return SBI_CALL_3(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE,
      SBI_SM_CALL_PLUGIN, SM_MULTIMEM_PLUGIN_ID, SM_MULTIMEM_CALL_GET_SIZE, size);
}

uintptr_t
sbi_query_multimem_addr(uintptr_t *addr) {
  return SBI_CALL_3(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE,
      SBI_SM_CALL_PLUGIN, SM_MULTIMEM_PLUGIN_ID, SM_MULTIMEM_CALL_GET_ADDR, addr);
}

uintptr_t
sbi_attest_enclave(void* report, void* buf, uintptr_t len) {
  return SBI_CALL_3(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_ATTEST_ENCLAVE, report, buf, len);
}

uintptr_t
sbi_get_sealing_key(uintptr_t key_struct, uintptr_t key_ident, uintptr_t len) {
  return SBI_CALL_3(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_GET_SEALING_KEY, key_struct, key_ident, len);
}

uintptr_t
sbi_claim_mmio(uintptr_t dev_string) {
  return SBI_CALL_1(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_CLAIM_MMIO, dev_string);
}

uintptr_t
sbi_release_mmio(uintptr_t dev_string) {
  return SBI_CALL_1(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_RELEASE_MMIO, dev_string);
}

#ifdef USE_CALLEE
uintptr_t
sbi_call_enclave(int eid, int type, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  return SBI_CALL_6(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_CALL_ENCLAVE,
             eid, type, arg0, arg1, arg2, arg3);
}

void
sbi_ret_enclave(uintptr_t ret) {
  SBI_CALL_1(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_RET_ENCLAVE, ret);
  __builtin_unreachable();
}

uintptr_t
sbi_register_handler(uintptr_t handler) {
  return SBI_CALL_1(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_REGISTER_HANDLER, handler);
}
#endif

uintptr_t
sbi_share_region(uintptr_t addr, size_t size) {
  return SBI_CALL_2(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_SHARE_REGION, addr, size);
}

uintptr_t
sbi_unshare_region(uintptr_t addr) {
  return SBI_CALL_1(SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE, SBI_SM_UNSHARE_REGION, addr);
}
