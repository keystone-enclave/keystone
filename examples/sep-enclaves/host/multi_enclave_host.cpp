#include <stdio.h>
#include <string.h>
//#include <thread>
#include "host/keystone.h"
#include "edge/edge_call.h"


#define OCALL_READ_PASSWORD 1
#define OCALL_PRINT_STRING 2
#define OCALL_PRINT_STRING2 3

void print_string_wrapper(void* buffer);
void read_password_wrapper(void* buffer);

unsigned long print_string(char* str) {
    printf("%s\n", str);
    return 0;
}

unsigned long read_password(char* buf, size_t buf_size) {
    printf("Veuillez entrer le mot de passe (max %lu caractères): ", buf_size - 1);
    fgets(buf, buf_size, stdin);
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
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
    register_call(OCALL_READ_PASSWORD, read_password_wrapper);
    register_call(OCALL_PRINT_STRING2, print_string_wrapper);

    enclave.run();
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Usage: %s <eapp1> <eapp2> <runtime> <device>\n", argv[0]);
        return 1;
    }

    printf("Lancement de la première enclave (password check)...\n");
    launch_enclave(argv[1], argv[3], argv[4]);

    printf("\nLancement de la deuxième enclave (multiplication)...\n");
    launch_enclave(argv[2], argv[3], argv[4]);

    /*printf("Lancement des enclaves en parallèle...\n");

    std::thread enclave1_thread(launch_enclave, argv[1], argv[3], argv[4]);
    std::thread enclave2_thread(launch_enclave, argv[2], argv[3], argv[4]);

    enclave1_thread.join();  // attendre la fin de la première enclave
    enclave2_thread.join();  // attendre la fin de la deuxième enclave

    printf("Les deux enclaves ont terminé.\n");*/

    return 0;
}

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

void read_password_wrapper(void* buffer) {
    struct edge_call* edge_call = (struct edge_call*)buffer;
    uintptr_t data_section = edge_call_data_ptr();
    char input[64];

    read_password(input, sizeof(input));
    memcpy((void*)data_section, input, sizeof(input));

    if (edge_call_setup_ret(edge_call, (void*)data_section, sizeof(input))) {
        edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
    } else {
        edge_call->return_data.call_status = CALL_STATUS_OK;
    }
}
