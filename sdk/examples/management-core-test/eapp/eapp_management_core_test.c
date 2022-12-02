#include "eapp_utils.h"
#include "string.h"
#include "edge_call.h"
#include <syscall.h>

#define OCALL_PRINT_STRING 1

unsigned long ocall_print_string(char* string);
void ipi_callback(struct sbi_scratch *scratch);

// void process(struct sbi_scratch *scratch) {
//   ocall_print_string("Enclave IPI process!\n");
// }

int main() {
  ocall_print_string("Enclave started.\n");
  reg_clock_ipi(ipi_callback);
  // print_time();
   
  EAPP_RETURN(0); // Will cause RUNTIME_SYSCALL_EXIT condition in SM
}

void ipi_callback(struct sbi_scratch *scratch) {
  ocall_print_string("Enclave IPI process!\n");
}

unsigned long ocall_print_string(char* string){
  unsigned long retval;
  ocall(OCALL_PRINT_STRING, string, strlen(string)+1, &retval, sizeof(unsigned long));
  return retval;
}
