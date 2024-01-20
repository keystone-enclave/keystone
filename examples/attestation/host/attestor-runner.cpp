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
  if (argc < 4 || argc > 9) {
    printf(
        "Usage: %s <eapp> <runtime> <loader> [--utm-size SIZE(K)] "
        "[--freemem-size SIZE(K)] [--sm-bin SM_BIN_PATH]\n",
        argv[0]);
    return 0;
  }

  int self_timing = 0;
  int load_only   = 0;

  size_t untrusted_size = 2 * 1024 * 1024;
  size_t freemem_size   = 48 * 1024 * 1024;
  bool retval_exist     = false;
  unsigned long retval  = 0;

  static struct option long_options[] = {
      {"utm-size", required_argument, 0, 'u'},
      {"freemem-size", required_argument, 0, 'f'},
      {"sm-bin", required_argument, 0, 's'},
      {0, 0, 0, 0}};

  char* eapp_file   = argv[1];
  char* rt_file     = argv[2];
  char* ld_file     = argv[3];
  char* sm_bin_file = NULL;

  int c;
  int opt_index = 4;
  while (1) {
    c = getopt_long(argc, argv, "u:f:s:", long_options, &opt_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        break;
      case 'u':
        untrusted_size = atoi(optarg) * 1024;
        break;
      case 'f':
        freemem_size = atoi(optarg) * 1024;
        break;
      case 's':
        sm_bin_file = optarg;
        break;
    }
  }

  if (sm_bin_file == NULL) {
    printf("--sm-bin is missing.\n");
    return 0;
  }

  Keystone::Params params;

  params.setFreeMemSize(freemem_size);
  params.setUntrustedSize(untrusted_size);

  Verifier verifier{params, eapp_file, rt_file, ld_file, sm_bin_file};
  verifier.run();

  return 0;
}
