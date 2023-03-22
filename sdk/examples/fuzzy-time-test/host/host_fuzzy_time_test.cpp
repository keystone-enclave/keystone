#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>

#include <stdio.h>
#include <edge_call.h>
#include <keystone.h>
#include <sys/mman.h>

#include "../test_consts.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

#define TRIAL_COUNT 3

void run_trials(char** argv, int trial_count, bool is_fuzzy_on);
uint64_t* run_trial(char** argv, bool is_fuzzy_on);
void spy(void* start_flag, void* target, uint64_t loop_counts[], int expected_writes);
bool test_basic_functionality(void* start_flag, void* expected_dest);

unsigned long print_string(char* str);
void print_string_wrapper(void* buffer);
#define OCALL_PRINT_STRING 1

int
main(int argc, char** argv) {
  run_trials(argv, TRIAL_COUNT, false);
  run_trials(argv, TRIAL_COUNT, true);
}

void run_trials(char** argv, int trial_count, bool is_fuzzy_on) {
  cout << "============================" << endl;
  cout << (is_fuzzy_on ? "FUZZY CLOCK ENABLED" : "FUZZY CLOCK DISABLED") << endl;
  cout << "============================" << endl;
  for (int i = 0; i < trial_count; i++) {
    cout << (is_fuzzy_on ? "FUZZY CLOCK ENABLED" : "FUZZY CLOCK DISABLED") << (" TRIAL ") << i << endl;
    cout << "-------------------------------------------------" << endl;
    uint64_t* spy_out = run_trial(argv, is_fuzzy_on);
  
    for (int i = 0; i < EXPECTED_WRITES; i++) {
      cout << "Loop " << i << ": " << spy_out[i] << " loops" << endl;
    }
  
    cout << "-------------------------------------------------" << endl << endl;
    munmap(spy_out, sizeof(uint64_t) * EXPECTED_WRITES);
  }
}

uint64_t* run_trial(char** argv, bool is_fuzzy_on) {
  uint64_t* spy_out = (uint64_t*)mmap(NULL, sizeof(uint64_t) * EXPECTED_WRITES, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  Keystone::Enclave enclave;
  Keystone::Params params;
  params.setFreeMemSize(1024 * 1024);
  params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 1024 * 1024);
  if (is_fuzzy_on)
    params.enableSecurityExtensions(SECURITY_EXTENSION_FUZZY_CLOCK);
  enclave.init(argv[1], argv[2], params);
  enclave.registerOcallDispatch(incoming_call_dispatch);
  /* We must specifically register functions we want to export to the
     enclave. */
  register_call(OCALL_PRINT_STRING, print_string_wrapper);
  edge_call_init_internals(
    (uintptr_t)enclave.getSharedBuffer(), enclave.getSharedBufferSize());
  void* shared_buff = enclave.getSharedBuffer();
  memset(shared_buff, 0x0, 1024 * 1024);

  int fork_result = fork();
  if (fork_result == -1) {
    printf("!!! FORK FAILED !!!\n");
    printf("!!! errno: %d\n", errno);
    while (1) { };
  }
  
  if (fork_result == 0) {
    cpu_set_t aset; CPU_ZERO(&aset); CPU_SET(1, &aset);
    int set_affinity_result = sched_setaffinity(0, sizeof(cpu_set_t), &aset);
    if (set_affinity_result != 0) {
      printf("!!! Spy proc failed to set affinity!\n");
    }
    
    printf("!!! Spy proc running on CPU %d\n", sched_getcpu());

    test_basic_functionality(
      shared_buff + START_FLAG_OFFSET_ONE, 
      shared_buff + ARBITRARY_OFFSET_ONE
    );
    
    spy(
      shared_buff + START_FLAG_OFFSET_TWO, 
      shared_buff + ARBITRARY_OFFSET_TWO, 
      spy_out, 
      EXPECTED_WRITES
    );

    sleep(99999);
  } else {
    cpu_set_t aset; CPU_ZERO(&aset); CPU_SET(2, &aset);
    int set_affinity_result = sched_setaffinity(0, sizeof(cpu_set_t), &aset);
    if (set_affinity_result != 0) {
      printf("!!! Host proc failed to set affinity!\n");
    }

    printf("!!! Host proc running on CPU %d\n", sched_getcpu());
    enclave.run();
    // sleep(10000);
    return spy_out;
  }
}

void spy(void* start_flag, void* target, uint64_t loop_counts[], int expected_writes) {
  volatile char* flag = (volatile char*)start_flag;
  flag[0] = 1;
  while (flag[0] == 1);
  
  volatile int* cur_target = (volatile int*)target;
  int curr_state = 0;
  for (int i = 0; i < expected_writes; i += 1) {
    uint64_t loops = 0;
    while (curr_state == 0) {
      loops += 1;
      curr_state = cur_target[i + 1];
    }
    loop_counts[i] = loops;
    curr_state = 0;
  }
}

bool test_basic_functionality(void* start_flag, void* expected_dest) {
  ((volatile char*)start_flag)[0] = 1;
  while (((volatile char*)start_flag)[0] == 1);

  string uw_string = UW;
  string fish_string = FISH;

  string actual = (char*)(expected_dest);
  // cout << "actual:\n" << actual << endl;
  assert(uw_string.compare(actual) == 0);

  actual = (char*)(expected_dest + UW_SIZE);
  // cout << "actual:\n" << actual << endl;
  assert(fish_string.compare(actual) == 0);

  cout << "Basic functionality tests passed." << endl;

  return true;
}


/***
 * An example call that will be exposed to the enclave application as
 * an "ocall". This is performed by an edge_wrapper function (below,
 * print_string_wrapper) and by registering that wrapper with the
 * enclave object (below, main).
 ***/
unsigned long
print_string(char* str) {
  return printf("Enclave said: \"%s\"\n", str);
}

/***
 * Example edge-wrapper function. These are currently hand-written
 * wrappers, but will have autogeneration tools in the future.
 ***/
void
print_string_wrapper(void* buffer) {
  /* Parse and validate the incoming call data */
  struct edge_call* edge_call = (struct edge_call*)buffer;
  uintptr_t call_args;
  unsigned long ret_val;
  size_t arg_len;
  if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
    edge_call->return_data.call_status = CALL_STATUS_BAD_OFFSET;
    return;
  }

  /* Pass the arguments from the eapp to the exported ocall function */
  ret_val = print_string((char*)call_args);

  /* Setup return data from the ocall function */
  uintptr_t data_section = edge_call_data_ptr();
  memcpy((void*)data_section, &ret_val, sizeof(unsigned long));
  if (edge_call_setup_ret(
          edge_call, (void*)data_section, sizeof(unsigned long))) {
    edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
  } else {
    edge_call->return_data.call_status = CALL_STATUS_OK;
  }

  /* This will now eventually return control to the enclave */
  return;
}