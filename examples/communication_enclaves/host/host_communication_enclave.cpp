#include <stdio.h>
#include <string.h>
#include "host/keystone.h"
#include "edge/edge_call.h"

#define OCALL_SEND_RESULT 1
#define OCALL_GET_RESULT 2
#define OCALL_PRINT_STRING 3


void print_string_wrapper(void* buffer);
void get_result_wrapper(void* buffer);
void send_result_wrapper(void* buffer);

int shared_result = 0;

unsigned long print_string(char* str) {
    printf("%s\n", str);
    return 0;
}

unsigned long read_result(int* out_result) {
    *out_result = shared_result;
    return 0;
}

unsigned long send_result(int result){
    shared_result = result;
    return 0;
}

void launch_enclave(const char* eapp_bin, const char* runtime, const char* device) {
    Keystone::Enclave enclave;
    Keystone::Params params;

    params.setFreeMemSize(1024 * 1024);
    params.setUntrustedSize(1024 * 1024);

    enclave.init(eapp_bin, runtime, device, params);
    edge_call_init_internals((uintptr_t)enclave.getSharedBuffer(), enclave.getSharedBufferSize());

    enclave.registerOcallDispatch(incoming_call_dispatch);
    register_call(OCALL_PRINT_STRING, print_string_wrapper);
    register_call(OCALL_GET_RESULT, get_result_wrapper);
    register_call(OCALL_SEND_RESULT, send_result_wrapper);

    enclave.run();
}
int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Usage: %s <eapp1> <eapp2> <runtime> <device>\n", argv[0]);
        return 1;
    }

    printf("Lancement de la première enclave (somme)...\n");
    launch_enclave(argv[1], argv[3], argv[4]);

    printf("\nLancement de la deuxième enclave (produit)...\n");
    launch_enclave(argv[2], argv[3], argv[4]);

    return 0;
}

//Wrappers 

void print_string_wrapper(void* buffer) {
    struct edge_call* edge_call = (struct edge_call*)buffer;
    uintptr_t call_args;
    size_t arg_len;
    unsigned long ret_val;

    if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
        edge_call->return_data.call_status = CALL_STATUS_BAD_OFFSET;
        return;
    }

    ret_val = print_string((char*)call_args);

    uintptr_t data_section = edge_call_data_ptr();
    memcpy((void*)data_section, &ret_val, sizeof(unsigned long));

    if (edge_call_setup_ret(edge_call, (void*)data_section, sizeof(unsigned long))) {
        edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
    } else {
        edge_call->return_data.call_status = CALL_STATUS_OK;
    }
}

void get_result_wrapper(void* buffer) {
    struct edge_call* edge_call = (struct edge_call*)buffer;
    int result;
    read_result(&result);
    //printf("[Host] Received result from enclave1: %d\n", result);  //for debug

    uintptr_t data_section = edge_call_data_ptr();
    memcpy((void*)data_section, &result, sizeof(result));

    if (edge_call_setup_ret(edge_call, (void*)data_section, sizeof(result))) {
        edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
    } else {
        edge_call->return_data.call_status = CALL_STATUS_OK;
    }
}

void send_result_wrapper(void* buffer){
    struct edge_call* edge_call = (struct edge_call*)buffer;
    uintptr_t call_args;
    size_t arg_len;

    if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
        edge_call->return_data.call_status = CALL_STATUS_BAD_OFFSET;
        return;
    }

    int result = *(int*)call_args;
    //printf("[Host] Sending result to enclave2: %d\n", result); //for debug
    send_result(result);

    uintptr_t data_section = edge_call_data_ptr();
    unsigned long ret_val;
    memcpy((void*)data_section, &ret_val, sizeof(ret_val));

    if (edge_call_setup_ret(edge_call, (void*)data_section, sizeof(result))) {
        edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
    } else {
        edge_call->return_data.call_status = CALL_STATUS_OK;
    }
}