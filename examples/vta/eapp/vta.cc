#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <tvm/runtime/crt/crt.h>
#include <tvm/runtime/crt/graph_executor.h>
#include <tvm/runtime/crt/packed_func.h>
#include <tvm/runtime/crt/page_allocator.h>
#include <unistd.h>
#include <vta/driver.h>
#include <string.h>

/* Fix some weird compile bugs */
#include <tvm/runtime/logging.h>

namespace tvm {
    namespace runtime {
        namespace detail {
            LogFatal::Entry& LogFatal::GetEntry() {
                static thread_local LogFatal::Entry result;
                return result;
            }
        }

        std::string Backtrace() {
            return nullptr;
        }
    }
}

extern "C" {

int TVMBackendGetFuncFromEnv(void* mod_node, const char* func_name, TVMFunctionHandle* func) {
    return TVMFuncGetGlobal(func_name, func);
}

void __attribute__((format(printf, 1, 2))) TVMLogf(const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
}

}


// Stolen from vta/runtime/runtime.h
#define VTA_DEBUG_DUMP_INSN (1 << 1)
#define VTA_DEBUG_DUMP_UOP (1 << 2)
#define VTA_DEBUG_SKIP_READ_BARRIER (1 << 3)
#define VTA_DEBUG_SKIP_WRITE_BARRIER (1 << 4)
#define VTA_DEBUG_FORCE_SERIAL (1 << 5)


#define TVM_CCALL(func)                                                              \
  do {                                                                               \
    tvm_crt_error_t ret = (tvm_crt_error_t) (func);                                  \
    if (ret != kTvmErrorNoError) {                                                   \
      fprintf(stderr, "%s: %d: error: %s\n", __FILE__, __LINE__, TVMGetLastError()); \
      exit(ret);                                                                     \
    }                                                                                \
  } while (0)

extern "C" {

extern void* VTABufferAlloc(size_t);
extern void VTABufferFree(void*);
extern void VTABufferCopy(const void*, size_t, void*, size_t, size_t, int);

typedef void* VTACommandHandle;
extern VTACommandHandle VTATLSCommandHandle();
extern void VTASetDebugMode(VTACommandHandle cmd, int debug_flag);

}

tvm_crt_error_t TVMPlatformMemoryAllocate(size_t num_bytes, DLDevice dev, void** out_ptr) {
    void *res = NULL;
    switch (dev.device_type) {
        case kDLCPU:
            res = std::malloc(num_bytes);
            break;

        case kDLExtDev:
            res = VTABufferAlloc(num_bytes);
            break;

        default:
            break;
    }

    if(res) {
        *out_ptr = res;
        return kTvmErrorNoError;
    }

    return kTvmErrorPlatformNoMemory;
}

tvm_crt_error_t TVMPlatformMemoryFree(void* ptr, DLDevice dev) {
    switch(dev.device_type) {
        case kDLCPU:
            std::free(ptr);
            break;

        case kDLExtDev:
            VTABufferFree(ptr);
            break;

        default:
            return kTvmErrorPlatformNoMemory;
    }

    return kTvmErrorNoError;
}

tvm_crt_error_t TVMPlatformMemoryCopy(const void* from, void *to, size_t size,
                                      DLDevice dev_from, DLDevice dev_to) {
    int kind_mask = 0;
    if (dev_from.device_type != kDLCPU) {
        kind_mask |= 2;
    }
    if (dev_to.device_type != kDLCPU) {
        kind_mask |= 1;
    }

    if(kind_mask) {
        VTABufferCopy(from, 0, to, 0, size, kind_mask);
    } else {
        memcpy(to, from, size);
    }

    return kTvmErrorNoError;
}

void TVMPlatformAbort(tvm_crt_error_t error_code) {
    // todo
    exit(-1);
}

tvm_crt_error_t TVMPlatformTimerStart() {
    return kTvmErrorFunctionCallNotImplemented;
}

tvm_crt_error_t TVMPlatformTimerStop(double* elapsed_time_seconds) {
    return kTvmErrorFunctionCallNotImplemented;
}

extern const char graph_json[];
extern unsigned int graph_json_len;

extern const char params_bin[];
extern unsigned int params_bin_len;

extern const char cat_bin[];
extern unsigned int cat_bin_len;

#define OUTPUT_LEN 1000
float output_storage[OUTPUT_LEN];

int VTASetDevice(TVMValue* args, int* type_codes, int num_args,
                 TVMValue* ret_value, int* ret_type_codes) {
    return 0;
}

int main() {
    DLDevice dev = {
            .device_type = kDLExtDev,
            .device_id = 0
    };

    TVMByteArray params = {
            .data = (char*)(params_bin),
            .size = params_bin_len
    };

//    VTASetDebugMode(VTATLSCommandHandle(),
//                    VTA_DEBUG_DUMP_INSN | VTA_DEBUG_DUMP_UOP);

    TVM_CCALL(TVMInitializeRuntime());

    TVMPackedFunc pf;
    TVMArgs args = TVMArgs_Create(NULL, NULL, 0);
    TVM_CCALL(TVMPackedFunc_InitGlobalFunc(&pf, "runtime.SystemLib", &args));
    TVM_CCALL(TVMPackedFunc_Call(&pf));

    TVMModuleHandle mod_syslib = TVMArgs_AsModuleHandle(&pf.ret_value, 0);

    // todo necessary?
    TVMFuncRegisterGlobal("__tvm_set_device", (TVMFunctionHandle) &VTASetDevice, 0);

    TVMGraphExecutor* graph_executor = NULL;
    TVM_CCALL(TVMGraphExecutor_Create((char *) graph_json, mod_syslib, &dev, &graph_executor));
    TVM_CCALL(TVMGraphExecutor_LoadParams(graph_executor, params.data, params.size));

    int64_t input_shape[4] = {1, 3, 224, 224};
    DLTensor input = {
            .data = (char *) cat_bin,
            .device = dev,
            .ndim = 4,
            .dtype = {
                    .code = kDLFloat,
                    .bits = 32, .lanes = 1
            },
            .shape = input_shape,
            .strides = NULL,
            .byte_offset = 0

    };

    TVMGraphExecutor_SetInput(graph_executor, "data", &input);
    TVMGraphExecutor_Run(graph_executor);

    int64_t output_shape[2] = {1, OUTPUT_LEN};
    DLTensor output = {
            .data = output_storage,
            .device = {
                    .device_type = kDLCPU,
                    .device_id = 0
            },
            .ndim = 2,
            .dtype = {
                    .code = kDLFloat,
                    .bits = 32, .lanes = 1
            },
            .shape = output_shape,
            .strides = NULL,
            .byte_offset = 0
    };

    TVMGraphExecutor_GetOutput(graph_executor, 0, &output);

    float max_iter = -FLT_MAX;
    int32_t max_index = -1;
    for (int i = 0; i < OUTPUT_LEN; ++i) {
        if (output_storage[i] > max_iter) {
            max_iter = output_storage[i];
            max_index = i;
        }
    }

    TVMGraphExecutor_Release(&graph_executor);
    fprintf(stderr, "Best result is %d with max %f\n", max_index, max_iter);
    return 0;
}
