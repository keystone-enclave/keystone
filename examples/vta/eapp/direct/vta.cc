#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tvm/runtime/crt/crt.h>
#include <tvm/runtime/crt/graph_executor.h>
#include <tvm/runtime/crt/packed_func.h>
#include <tvm/runtime/crt/page_allocator.h>

#include "platform.h"
#include "images.h"

//#define VERBOSE

extern const char graph_json[];
extern unsigned int graph_json_len;

extern const char params_bin[];
extern unsigned int params_bin_len;

extern const char imgs_xz[];
extern unsigned int imgs_xz_len;

#define BATCH      1
#define OUTPUT_LEN 1000
float output_storage[BATCH * OUTPUT_LEN];
struct argmax {
    int i;
    float val;
};
struct argmax output_argmax[OUTPUT_LEN];
float input_storage[BATCH * 3 * 224 * 224];


int argmax_compare(const void *a, const void *b) {
    struct argmax *arg_a = (struct argmax *) a,
            *arg_b = (struct argmax *) b;

    if(arg_a->val < arg_b->val) {
        return 1;
    } else {
        return -1;
    }
}

#define read_csr(reg) __extension__({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

int main() {
    DLDevice dev = {
            .device_type = kDLExtDev,
            .device_id = 0
    };

    TVMByteArray params = {
            .data = (char*)(params_bin),
            .size = params_bin_len
    };

    vta_examples_init_runtime();
    vta_set_debug(0);

    TVMGraphExecutor* graph_executor = NULL;
    vta_examples_init_executor(&graph_executor, &dev, (char *) graph_json);
    TVM_CCALL(TVMGraphExecutor_LoadParams(graph_executor, params.data, params.size));

    int64_t input_shape[4] = {BATCH, 3, 224, 224};
    DLTensor input = {
            .data = input_storage,
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

    int64_t output_shape[2] = {BATCH, OUTPUT_LEN};
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

    int i, b, img = 0;
    uint64_t start, end;

    while(true)
    {
        i = get_next_image(input_storage, sizeof(input_storage));
        if(i < 0) {
            fprintf(stderr, "Couldn't get next image\n");
            return -1;
        }

        TVMGraphExecutor_SetInput(graph_executor, "data", &input);
        start = read_csr(cycle);
        TVMGraphExecutor_Run(graph_executor);
        end = read_csr(cycle);
        TVMGraphExecutor_GetOutput(graph_executor, 0, &output);

        // Output data is now in output_storage
        for(b = 0; b < BATCH; b++) {
            fprintf(stderr, "\nImage %i (%li cycles)\n", img, end - start);
            for(i = 0; i < OUTPUT_LEN; i++) {
                output_argmax[i].i = i;
                output_argmax[i].val = output_storage[OUTPUT_LEN * b + i];

#ifdef VERBOSE
                if(i == 0) {
                    fprintf(stderr, "Raw:");
                }
                if(i % 8 == 0) {
                    fprintf(stderr, "\n\t");
                }
                fprintf(stderr, "%0.3f\t", output_storage[OUTPUT_LEN * b + i]);
#endif
            }

            qsort(output_argmax, OUTPUT_LEN, sizeof(struct argmax), argmax_compare);

#ifdef VERBOSE
            fprintf(stderr, "\nArgmax:");
            for(i = 0; i < OUTPUT_LEN; i++) {

                if(i % 8 == 0) {
                    fprintf(stderr, "\n\t");
                }
                fprintf(stderr, "%0.3f (%i)\t", output_argmax[i].val, output_argmax[i].i);
            }
#else
            for(i = 0; i < 5; i++) {
                fprintf(stderr, "%i\t", output_argmax[i].i);
            }
            fprintf(stderr, "\n");
#endif

            img++;
        }
    }

    TVMGraphExecutor_Release(&graph_executor);
    return 0;
}
