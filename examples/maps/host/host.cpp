
#include <thread>

#include "edge/edge_call.h"
#include "host/keystone.h"

using namespace Keystone;


int main(int argc, char **argv) {
  Enclave encl;
  Params params;

  params.setFreeMemSize(1024 * 1024);
  params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 1024 * 1024);

  printf("Loading eapp %s with rt %s\n", argv[1], argv[2]);
  encl.init(argv[1], argv[2], params);
  encl.run();
  return 0;
}