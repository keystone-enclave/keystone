//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <getopt.h>
#include <keystone.h>
#include <cstdio>
#include <iostream>
#include "gtest/gtest.h"

#define EYRIE_RT "eyrie-rt"
#define TEST_EAPP "stack.eapp_riscv"
#define FAKE_EAPP "fake_file.riscv"
#define FAKE_RT "fake-rt"
#define MD_SIZE 64

size_t untrusted_size = 2 * 1024 * 1024;
size_t freemem_size   = 48 * 1024 * 1024;
uintptr_t utm_ptr     = (uintptr_t)DEFAULT_UNTRUSTED_PTR;

using Keystone::Enclave;
using Keystone::Error;
using Keystone::Params;

TEST(Enclave_Init, DeterministicMeasurement) {
  /* Measures enclave using a simulated enclave
   *
   * */
  Enclave enclave1, enclave2;
  Params params1, params2;

  params1.setFreeMemSize(untrusted_size);
  params1.setUntrustedMem(utm_ptr, untrusted_size);
  params1.setSimulated(true);

  params2.setFreeMemSize(untrusted_size);
  params2.setUntrustedMem(utm_ptr, untrusted_size);
  params2.setSimulated(true);

  EXPECT_EQ(Error::Success, enclave1.init(TEST_EAPP, EYRIE_RT, params1));
  EXPECT_EQ(Error::Success, enclave2.init(TEST_EAPP, EYRIE_RT, params2));
  EXPECT_EQ(0, memcmp(enclave1.getHash(), enclave2.getHash(), MD_SIZE));
  EXPECT_EQ(Error::Success, enclave1.destroy());
  EXPECT_EQ(Error::Success, enclave2.destroy());
}

TEST(Enclave_Init, InvalidRT) {
  /* Tests enclave with a non-existant runtime
   *
   * */
  Enclave enclave;
  Params params;

  params.setFreeMemSize(untrusted_size);
  params.setUntrustedMem(utm_ptr, untrusted_size);
  params.setSimulated(true);

  EXPECT_EQ(enclave.init(TEST_EAPP, FAKE_RT, params), Error::FileInitFailure);
}

TEST(Enclave_Init, InvalidEAPP) {
  /* Tests enclave with a non-existant enclave app
   *
   * */
  Enclave enclave;
  Params params;

  params.setFreeMemSize(untrusted_size);
  params.setUntrustedMem(utm_ptr, untrusted_size);
  params.setSimulated(true);

  EXPECT_EQ(enclave.init(FAKE_EAPP, EYRIE_RT, params), Error::FileInitFailure);
}

TEST(Enclave_Run, RunTest) {
  /* Tests running the enclave after initialization
   *
   * */
  Enclave enclave;
  Params params;

  params.setFreeMemSize(untrusted_size);
  params.setUntrustedMem(utm_ptr, untrusted_size);
  params.setSimulated(true);

  EXPECT_EQ(enclave.init(TEST_EAPP, EYRIE_RT, params), Error::Success);
  EXPECT_EQ(enclave.run(), Error::Success);
}

int
main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
