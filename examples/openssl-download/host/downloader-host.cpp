#include <thread>

#include "edge/edge_call.h"
#include "host/keystone.h"

using namespace Keystone;

// Give some extra malloc-able space for the eapp to download larger files
#define MULTIPLIER 5
int main(int argc, char **argv) {
  Enclave encl;
  Params params;
  
  params.setFreeMemSize(1024 * 1024 * MULTIPLIER);
  params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 1024 * 1024);

  // 1697155200 == Date and time (GMT): Friday, October 13, 2023 12:00:00 AM
  params.setTimeSinceUnixEpoch(1697155200);

  printf("Loading eapp %s with rt %s\n", argv[1], argv[2]);
  encl.init(argv[1], argv[2], params);

  encl.registerOcallDispatch(incoming_call_dispatch);
  edge_call_init_internals(
      (uintptr_t)encl.getSharedBuffer(), encl.getSharedBufferSize());

  encl.run();
  return 0;
}
