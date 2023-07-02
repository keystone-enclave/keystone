#include "eapp_utils.h"
#include "string.h"
#include "edge_call.h"
#include "../test_consts.h"
#include <syscall.h>

#define OCALL_PRINT_STRING 1

unsigned long ocall_print_string(char* string);
void loop(unsigned long u);

int main() {
  // Basic functionality tests
  int start_flag = 0;
  while (start_flag == 0) {
    // door stucc
    copy_from_shared(&start_flag, START_FLAG_OFFSET_ONE, sizeof(char));
  }
  char* uw = UW;
  char* fish = FISH;
  write_to_shared((void*)uw, (uintptr_t)ARBITRARY_OFFSET_ONE, UW_SIZE);
  write_to_shared((void*)fish, (uintptr_t)ARBITRARY_OFFSET_ONE + UW_SIZE, FISH_SIZE);
  start_flag = 0;
  write_to_shared((void*)&start_flag, START_FLAG_OFFSET_ONE, sizeof(char));

  // Fuzzing tests
  while (start_flag == 0) {
    copy_from_shared(&start_flag, START_FLAG_OFFSET_TWO, sizeof(char));
  }
  start_flag = 0;
  write_to_shared(&start_flag, START_FLAG_OFFSET_TWO, sizeof(char));
  // uint64_t n[] = { 1, 2, 4, 6, 8, 10, 12 };
  uint32_t n[] = { 1, 2, 4, 8, 16, 32, 64 };
  int i = 1;
  write_to_shared((void*)&i, (uintptr_t)ARBITRARY_OFFSET_TWO + (sizeof(int) * (i)), sizeof(int));
  i += 1;
  while (i < EXPECTED_WRITES + 1) {
    loop(LOOP_CONST * n[i - 2]);
    write_to_shared((void*)&i, (uintptr_t)ARBITRARY_OFFSET_TWO + (sizeof(int) * (i)), sizeof(int));
    i += 1;
  }
  EAPP_RETURN(0); // Will cause RUNTIME_SYSCALL_EXIT condition in SM
}

void loop(uint64_t u) {
  uint64_t i = 0;
  while (i < u) {
    i += 1;
  }
}

unsigned long ocall_print_string(char* string){
  unsigned long retval;
  ocall(OCALL_PRINT_STRING, string, strlen(string)+1, &retval, sizeof(unsigned long));
  return retval;
}
