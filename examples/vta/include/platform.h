#ifndef KEYSTONE_EXAMPLES_PLATFORM_H
#define KEYSTONE_EXAMPLES_PLATFORM_H

#define TVM_CCALL(func)                                                              \
  do {                                                                               \
    tvm_crt_error_t ret = (tvm_crt_error_t) (func);                                  \
    if (ret != kTvmErrorNoError) {                                                   \
      fprintf(stderr, "%s: %d: error: %s\n", __FILE__, __LINE__, TVMGetLastError()); \
      exit(ret);                                                                     \
    }                                                                                \
  } while (0)

void vta_examples_init_runtime(void);
void vta_examples_init_executor(TVMGraphExecutor **, DLDevice *, char *);

#define VTA_DEBUG_DUMP_INSN (1 << 1)
#define VTA_DEBUG_DUMP_UOP (1 << 2)
#define VTA_DEBUG_SKIP_READ_BARRIER (1 << 3)
#define VTA_DEBUG_SKIP_WRITE_BARRIER (1 << 4)
#define VTA_DEBUG_FORCE_SERIAL (1 << 5)

void vta_set_debug(int debug_flag);

#endif //KEYSTONE_EXAMPLES_PLATFORM_H
