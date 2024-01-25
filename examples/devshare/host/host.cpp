
#include "edge/edge_call.h"
#include "host/keystone.h"

using namespace Keystone;

int main(int argc, char **argv) {
  Enclave enclave;
  Params params;

  params.setFreeMemSize(1024 * 1024);
  params.setUntrustedSize(1024 * 1024);

  enclave.init(argv[1], argv[2], argv[3], params);

  enclave.registerOcallDispatch(incoming_call_dispatch);
  edge_call_init_internals(
      (uintptr_t)enclave.getSharedBuffer(), enclave.getSharedBufferSize());

  enclave.run();
  return 0;
}