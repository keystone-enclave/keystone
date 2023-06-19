//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <getopt.h>
#include <stdlib.h>

#include <cerrno>
#include <cstdio>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#include "host/keystone.h"
#include "verifier/report.h"

#include "verifier.h"

int
main(int argc, char** argv) {
  if (argc < 3 || argc > 8) {
    printf(
        "Usage: %s <eapp> <runtime> [--utm-size SIZE(K)] [--freemem-size "
        "SIZE(K)] [--utm-ptr 0xPTR] [--sm-bin SM_BIN_PATH]\n",
        argv[0]);
    return 0;
  }

  int self_timing = 0;
  int load_only   = 0;

  size_t untrusted_size = 2 * 1024 * 1024;
  size_t freemem_size   = 48 * 1024 * 1024;
  uintptr_t utm_ptr     = (uintptr_t)DEFAULT_UNTRUSTED_PTR;
  bool retval_exist     = false;
  unsigned long retval  = 0;

  static struct option long_options[] = {
      {"utm-size", required_argument, 0, 'u'},
      {"utm-ptr", required_argument, 0, 'p'},
      {"freemem-size", required_argument, 0, 'f'},
      {"sm-bin", required_argument, 0, 's'},
      {0, 0, 0, 0}};

  char* eapp_file   = argv[1];
  char* rt_file     = argv[2];
  char* sm_bin_file = NULL;

  int c;
  int opt_index = 3;
  while (1) {
    c = getopt_long(argc, argv, "u:p:f:s:", long_options, &opt_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        break;
      case 'u':
        untrusted_size = atoi(optarg) * 1024;
        break;
      case 'p':
        utm_ptr = strtoll(optarg, NULL, 16);
        break;
      case 'f':
        freemem_size = atoi(optarg) * 1024;
        break;
      case 's':
        sm_bin_file = optarg;
        break;
    }
  }

  Keystone::Params params;

  params.setFreeMemSize(freemem_size);
  params.setUntrustedMem(utm_ptr, untrusted_size);

  Verifier verifier{params, eapp_file, rt_file, sm_bin_file};
  verifier.run();

  return 0;
}
