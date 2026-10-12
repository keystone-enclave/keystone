#include <edge_call.h>
#include <keystone.h>
#include <ctime> 

#define OCALL_PRINT_STRING 1  


void* enclave_ptr = nullptr;
unsigned long
print_string(char* str);
void
print_string_wrapper(void* buffer);
unsigned long print_string(char* str) {
    printf("%s\n", str); 
    return 0;
}

int
main(int argc, char** argv) {
  Keystone::Enclave enclave;
  Keystone::Params params;
  enclave_ptr = &enclave;
  params.setFreeMemSize(2048 * 2048);
  params.setUntrustedSize(512 * 512);

  enclave.init(argv[1], argv[2], argv[3], params);
  enclave.setEnclaveConfig(2); 
   // Enregistrer les appels "ocall"
    enclave.registerOcallDispatch(incoming_call_dispatch);

   /* Enregistrer la fonction d'exportation de l'ocall pour afficher un texte */
    register_call(OCALL_PRINT_STRING, print_string_wrapper);
    edge_call_init_internals(
        (uintptr_t)enclave.getSharedBuffer(), enclave.getSharedBufferSize());
        
    //clock_t start_time = clock();
    enclave.run();
    //clock_t end_time = clock();
    //double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    //printf("Temps d'exécution de l'enclave : %.6f secondes\n", execution_time);
    return 0;
}

void print_string_wrapper(void* buffer) {
    struct edge_call* edge_call = (struct edge_call*)buffer;
    uintptr_t call_args;
    unsigned long ret_val;
    size_t arg_len;

    // Extraire l'argument (pointeur vers le texte)
    if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
        edge_call->return_data.call_status = CALL_STATUS_BAD_OFFSET;
        return;
    }

    // Passer l'argument texte à la fonction print_string
    ret_val = print_string((char*)call_args);

    // Mettre en place les données de retour
    uintptr_t data_section = edge_call_data_ptr();
    memcpy((void*)data_section, &ret_val, sizeof(unsigned long));

    // Finaliser l'appel et gérer les retours
    if (edge_call_setup_ret(edge_call, (void*)data_section, sizeof(unsigned long))) {
        edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
    } else {
        edge_call->return_data.call_status = CALL_STATUS_OK;
    }

    return;
}