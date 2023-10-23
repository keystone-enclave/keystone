#ifndef KEYSTONE_EXAMPLES_INTERFACE_H
#define KEYSTONE_EXAMPLES_INTERFACE_H

// Used by linker
#define OPERATOR_LIB_ADDR 0x800000

// Whether to use the mapped implementation
#define USE_MAPPED              1

// Verbose printing
//#define VERBOSE 1

#define EID_DRIVER_ENCLAVE      0
#define EID_CONSUMER_ENCLAVE    1

#ifndef __PREPROCESSING__

#ifdef __cplusplus
extern "C" {
#endif

#include <tvm/runtime/crt/module.h>
#include <tvm/runtime/c_backend_api.h>

#define read_csr(reg) __extension__({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

/* Functionality for interfacing with operator library */

typedef void *VTACommandHandle;

struct cmd_table {
    void                (*TVMAPISetLastError)(const char *);
    void *              (*TVMBackendAllocWorkspace)(int, int, uint64_t, int, int);
    int                 (*TVMBackendFreeWorkspace)(int, int, void *);
    int                 (*TVMBackendGetFuncFromEnv)(void *, const char *, TVMFunctionHandle *);
    int                 (*TVMBackendParallelLaunch)(FTVMParallelLambda, void *, int);
    int                 (*TVMFuncCall)(TVMFunctionHandle, TVMValue *, int *, int, TVMValue *, int *);

    void *              (*VTABufferCPUPtr)(VTACommandHandle, void *);
    int                 (*VTADepPop)(VTACommandHandle, int, int);
    int                 (*VTADepPush)(VTACommandHandle, int, int);
    void                (*VTALoadBuffer2D)(void *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
                                            uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
    int                 (*VTAPushALUOp)(void **, int (*)(void *), void *, int);
    int                 (*VTAPushGEMMOp)(void **, int (*)(void *), void *, int);
    void                (*VTAStoreBuffer2D)(uint32_t, uint32_t, void *, uint32_t, uint32_t, uint32_t, uint32_t);
    void                (*VTASynchronize)(uint32_t);
    VTACommandHandle    (*VTATLSCommandHandle)(void);
    void                (*VTAUopLoopBegin)(uint32_t, uint32_t, uint32_t, uint32_t);
    void                (*VTAUopLoopEnd)(void);
    void                (*VTAUopPush)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
};

typedef const TVMModule *(*syslib_entry_fn)(void);
typedef syslib_entry_fn (*initialize_comms_fn)(const struct cmd_table *);

/* State machine */

typedef enum {
    STATE_READY,
    STATE_RECV_GRAPH,
    STATE_RECV_OPERATORS,
    STATE_RECV_PARAMS,
    STATE_RECV_IMAGE,
    STATE_SEND_RESULT
} vta_state_t;

typedef enum {
    CMD_GRAPH,
    CMD_OPERATOR,
    CMD_PARAMS,
    CMD_IMAGE,
    CMD_RESULT,
    CMD_RESET
} vta_cmd_t;

#ifdef __cplusplus
}
#endif

#endif // __PREPROCESSING__

#endif //KEYSTONE_EXAMPLES_INTERFACE_H
