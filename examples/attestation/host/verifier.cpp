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
  Host host(params, eapp_file, rt_file, ld_file);
  Keystone::Enclave::Checkpoint checkpoint_additions;
  {
    // create a random file, not used by enclave
    const std::string extra_filename = "extra_file.txt";
    // random contents
    std::string extra_contents;
    int extra_contents_length = 10;
    extra_contents.reserve(extra_contents_length);
    for (int i = 0; i < extra_contents_length; i++) {
      extra_contents += (char) (random() % 128);
    }
    // write to file
    FILE* extra_file = fopen(extra_filename.c_str(), "w");
    if (!extra_file)
      throw std::runtime_error(
          "Error opening extra_file: " + extra_filename + ", " +
          std::strerror(errno));
    if (fwrite(extra_contents.data(), 1, extra_contents.size(), extra_file)
        != extra_contents.size()) {
      throw std::runtime_error(
          "Error writing extra_file: " + extra_filename + ", " +
          std::strerror(errno));
      }
    fclose(extra_file);
    // use Keystone::Enclave to create the delta, and add to existing enclave
    Keystone::Enclave& enclave = host.getEnclave();
    enclave.startDelta();
    enclave.addResidentResource(extra_filename.c_str(), 0,
      extra_filename.c_str(), 1);
    checkpoint_additions = enclave.makeDeltaCheckpoint();
  }
  Report report = host.run(nonce);
  verify_report(report, nonce, checkpoint_additions);
}

void
Verifier::verify_report(Report& report, const std::string& nonce,
    Keystone::Enclave::Checkpoint checkpoint_additions) {
  debug_verify(report, _sanctum_dev_public_key);

  byte expected_enclave_hash[MDSIZE];
  compute_expected_enclave_hash(expected_enclave_hash, checkpoint_additions);

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
Verifier::compute_expected_enclave_hash(byte* expected_enclave_hash,
    Keystone::Enclave::Checkpoint checkpoint_additions) {
  // create base
  Keystone::Enclave enclave(params);
  enclave.addStandard(
    eapp_file.c_str(), rt_file.c_str(), ld_file.c_str());
  Keystone::Enclave::Checkpoint checkpoint = enclave.makeCheckpoint();
  // add additions & calculate hash
  // in reality, would also validate checkpoint_additions is allowed
  checkpoint.addFromCheckpoint(checkpoint_additions);
  checkpoint.measurement((char *) expected_enclave_hash);
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
    FILE* sm_bin = fopen(sm_bin_file.c_str(), "rb");
    if (!sm_bin)
      throw std::runtime_error(
          "Error opening sm_bin_file: " + sm_bin_file + ", " +
          std::strerror(errno));
    if (fread(sm_content.data(), 1, sm_content.size(), sm_bin) <= 0)
      throw std::runtime_error(
          "Error reading sm_bin_file: " + sm_bin_file + ", " +
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
