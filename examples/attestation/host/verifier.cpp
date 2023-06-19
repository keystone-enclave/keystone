//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "verifier.h"

#include <getopt.h>
#include <stdlib.h>

#include <cerrno>
#include <cstdio>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#include "host.h"
#include "host/hash_util.hpp"
#include "host/keystone.h"
#include "verifier/report.h"
#include "verifier/test_dev_key.h"

void
Verifier::run() {
  const std::string nonce = std::to_string(random() % 0x100000000);
  Host host(params_, eapp_file_, rt_file_);
  Report report = host.run(nonce);
  verify_report(report, nonce);
}

void
Verifier::verify_report(Report& report, const std::string& nonce) {
  debug_verify(report, _sanctum_dev_public_key);

  byte expected_enclave_hash[MDSIZE];
  compute_expected_enclave_hash(expected_enclave_hash);

  byte expected_sm_hash[MDSIZE];
  compute_expected_sm_hash(expected_sm_hash);

  verify_hashes(
      report, expected_enclave_hash, expected_sm_hash, _sanctum_dev_public_key);

  verify_data(report, nonce);
}

void
Verifier::verify_hashes(
    Report& report, const byte* expected_enclave_hash,
    const byte* expected_sm_hash, const byte* dev_public_key) {
  if (report.verify(expected_enclave_hash, expected_sm_hash, dev_public_key)) {
    printf("Enclave and SM hashes match with expected.\n");
  } else {
    printf(
        "Either the enclave hash or the SM hash (or both) does not "
        "match with expeced.\n");
    report.printPretty();
  }
}

void
Verifier::verify_data(Report& report, const std::string& nonce) {
  if (report.getDataSize() != nonce.length() + 1) {
    const char error[] =
        "The size of the data in the report is not equal to the size of the "
        "nonce initially sent.";
    printf(error);
    report.printPretty();
    throw std::runtime_error(error);
  }

  if (0 == strcmp(nonce.c_str(), (char*)report.getDataSection())) {
    printf("Returned data in the report match with the nonce sent.\n");
  } else {
    printf("Returned data in the report do NOT match with the nonce sent.\n");
  }
}

void
Verifier::compute_expected_enclave_hash(byte* expected_enclave_hash) {
  Keystone::Enclave enclave;
  Keystone::Params simulated_params = params_;
  simulated_params.setSimulated(true);
  // This will cause validate_and_hash_enclave to be called when
  // isSimulated() == true.
  enclave.init(eapp_file_.c_str(), rt_file_.c_str(), simulated_params);
  memcpy(expected_enclave_hash, enclave.getHash(), MDSIZE);
}

void
Verifier::compute_expected_sm_hash(byte* expected_sm_hash) {
  // It is important to make sure the size of the SM buffer we are
  // measuring is the same as the size of the SM buffer allocated by
  // the bootloader. See keystone/bootrom/bootloader.c for how it is
  // computed in the bootloader.
  const size_t sanctum_sm_size = 0x1ff000;
  std::vector<byte> sm_content(sanctum_sm_size, 0);

  {
    // Reading SM content from file.
    FILE* sm_bin = fopen(sm_bin_file_.c_str(), "rb");
    if (!sm_bin)
      throw std::runtime_error(
          "Error opening sm_bin_file_: " + sm_bin_file_ + ", " +
          std::strerror(errno));
    if (fread(sm_content.data(), 1, sm_content.size(), sm_bin) <= 0)
      throw std::runtime_error(
          "Error reading sm_bin_file_: " + sm_bin_file_ + ", " +
          std::strerror(errno));
    fclose(sm_bin);
  }

  {
    // The actual SM hash computation.
    hash_ctx_t hash_ctx;
    hash_init(&hash_ctx);
    hash_extend(&hash_ctx, sm_content.data(), sm_content.size());
    hash_finalize(expected_sm_hash, &hash_ctx);
  }
}

void
Verifier::debug_verify(Report& report, const byte* dev_public_key) {
  if (report.checkSignaturesOnly(dev_public_key)) {
    printf("Attestation report SIGNATURE is valid\n");
  } else {
    printf("Attestation report is invalid\n");
  }
}
