#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>
#include <ctime>
#include <unistd.h>
#include <assert.h>
// Keystone
#include <edge_call.h>
#include <keystone.h>
#include "../test_consts.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::thread;

string FISH_STRING = FISH;
string UW_STRING = UW;

bool test_basic_functionality(uintptr_t sharedBufferPtr);
void spy(uintptr_t target, uint64_t times[], int states[], int expected_writes);
uint64_t* test_fuzzing(char** argv);
unsigned long print_string(char* str);
void print_string_wrapper(void* buffer);
#define OCALL_PRINT_STRING 1

int
main(int argc, char** argv) {
  for (int i = 0; i < 5; i++) {
    cout << "TRIAL " << i << endl;
    cout << "-------------------------------------------------" << endl;
    uint64_t* spy_out = test_fuzzing(argv);

    for (int i = 0; i < EXPECTED_WRITES; i++) {
    cout << "Time point " << i << ": " << spy_out[i] << endl;
    }
    for (int i = 0; i < EXPECTED_WRITES-1; i++) {
      cout << "pt " << i+1 << " - pt " << i << ": ~" << (spy_out[i+1] - spy_out[i]) / 1000000.0 << " ms" << endl;
    }
    
    cout << endl;
    float loop_const_time = (spy_out[1] - spy_out[0]);
    cout << LOOP_CONST << " loops took ~" << loop_const_time / 1000000.0 << " ms" << endl;
    for (int i = 1; i < EXPECTED_WRITES-1; i++) {
      float factor = (spy_out[i+1] - spy_out[i]) / loop_const_time;
      cout << "Loop " << i << " looped ~" << factor * LOOP_CONST << " times." << endl;
    }

    cout << "-------------------------------------------------" << endl << endl;
    free(spy_out);
  }

  return 0;
}

uint64_t* test_fuzzing(char** argv) {
  Keystone::Enclave enclave;
  Keystone::Params params;
  params.setFreeMemSize(1024 * 1024);
  params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 1024 * 1024);
  enclave.init(argv[1], argv[2], params);
  enclave.registerOcallDispatch(incoming_call_dispatch);
  /* We must specifically register functions we want to export to the
     enclave. */
  register_call(OCALL_PRINT_STRING, print_string_wrapper);
  edge_call_init_internals(
    (uintptr_t)enclave.getSharedBuffer(), enclave.getSharedBufferSize());

  void* shared_buff = enclave.getSharedBuffer();
  memset(shared_buff, 0x0, 1024 * 1024);

  uintptr_t basic_tests_expected_dst = (uintptr_t)shared_buff + ARBITRARY_OFFSET_ONE;
  uintptr_t spy_tests_expected_dst = (uintptr_t)shared_buff + ARBITRARY_OFFSET_TWO;

  uint64_t* spy_out = (uint64_t*)malloc(2048);
  int* spy_states = (int*)malloc(2048);

  memset(spy_out, 0x0, 2048);
  memset(spy_states, 0x0, 2048);

  thread spy_thread(spy, spy_tests_expected_dst, spy_out, spy_states, EXPECTED_WRITES);

  enclave.run();

  spy_thread.join();

  if (test_basic_functionality((uintptr_t)basic_tests_expected_dst)) {
    cout << "Basic functionality tests pass." << endl;
  }
  cout << "Observed states: ";
  for (int i = 0; i <= EXPECTED_WRITES; i++) {
    cout << spy_states[i] << ", ";
  }
  free(spy_states);
  return spy_out;
}

void spy(uintptr_t target, uint64_t times[], int states[], int expected_writes) {
  times[0] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  states[0] = *(int*)target;
  int curr_state = 0;

  for (int i = 1; i <= expected_writes; i++) {
    while (curr_state == 0) {
      curr_state = *((int*)(target) + (i-1));
    }
    // cout << "observed: " << curr_state << endl;
    times[i-1] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    states[i] = curr_state;
    curr_state = 0;
  }
}

bool test_basic_functionality(uintptr_t expected_dest) {
  string actual = (char*)(expected_dest);
  assert(FISH_STRING.compare(actual) == 0);

  actual = (char*)(expected_dest + FISH_SIZE);
  assert(UW_STRING.compare(actual) == 0);

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
