#include "eapp_utils.h"
#include "string.h"
#include "edge_call.h"
#include "../test_consts.h"
#include <syscall.h>

#define OCALL_PRINT_STRING 1

unsigned long ocall_print_string(char* string);
void loop(unsigned long u);

// toread: fuzzying: secure VAX retrospective, "trusted browsers uncertain times," 

// TODO(chungmcl): Add option for turning fuzzy on/off
// look at keystone/linux-keystone-driver/keystone-ioctl.c, 
// keystone SDK (have a way for host to turn on flags), 
// keystone SM (check for flags, turn flags on)

// TODO(chungmcl): Write a test
// Make a host app that
// forks into two threads --
// one thread makes eapp do stuff (write, etc.)
// other thread watches over shared mem
// to see that stuff is written as expected
// at the right time

// We want to run a "failed" test (one which DOESN'T use the timing_buffer)
// Use that as a "benchmark" to compare a "successful" test against (show that the timing_buffer does indeed hide)

int main() {
  // Basic functionality tests
  ocall_print_string("ocall_print_string: I'm fish");
  pause_ms(4000);
  char* fish = FISH;
  char* uw = UW;
  write_to_shared((void*)fish, (uintptr_t)ARBITRARY_OFFSET_ONE, FISH_SIZE);
  write_to_shared((void*)uw, (uintptr_t)ARBITRARY_OFFSET_ONE + FISH_SIZE, UW_SIZE);

  // Fuzzing tests
  pause_ms(1000);
  // uint64_t n[] = { 1, 2, 4, 6, 8, 10, 12 };
  uint64_t n[] = { 1, 2, 4, 8, 16, 32, 64 };
  int i = 1;
  write_to_shared((void*)&i, (uintptr_t)ARBITRARY_OFFSET_TWO + (sizeof(int) * (i-1)), sizeof(int));
  i += 1;
  while (i <= EXPECTED_WRITES) {
    loop(LOOP_CONST * n[i - 2]);
    write_to_shared((void*)&i, (uintptr_t)ARBITRARY_OFFSET_TWO + (sizeof(int) * (i-1)), sizeof(int));
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
