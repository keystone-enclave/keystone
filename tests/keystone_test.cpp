//
// Created by Alex Thomas on 8/31/19.
//

#include <getopt.h>
#include <iostream>
#include <cstdio>
#include "gtest/gtest.h"
#include "keystone.h"

#define EYRIE_RT "test_binary/eyrie-rt"
#define TEST_EAPP "test_binary/stack.eapp_riscv"
#define FAKE_EAPP "test_binary/fake_file.riscv"
#define FAKE_RT "test_binary/fake-rt"
#define MD_SIZE 64

char const *ref_hash = "\x10\xc7\xb0\x61\xb0\x0d\xcb\x90\x5d\x5a\xa0\x88\x92\xba\xbc\x1a\x0c\xcb\x13\xf4\xa1\x5f\xe4\xfc\x63\xf8\xd0\x26\xe3\xc4\xfa\xe1\x77\x9f\xc6\xff\x8a\x3f\x40\x1e\xd9\xef\xad\xeb\xf9\x30\x70\xf2\x31\x49\xa5\x38\x42\x93\xcf\x27\xcb\xc2\xa4\x7a\xce\xe4\xba\x8c";

size_t untrusted_size = 2*1024*1024;
size_t freemem_size = 48*1024*1024;
uintptr_t utm_ptr = (uintptr_t)DEFAULT_UNTRUSTED_PTR;

TEST(Keystone_Init, ValidMeasure) {
  /* Measures enclave using a simulated enclave
   *
   * */
  Keystone enclave;
  Params params;

  params.setFreeMemSize(untrusted_size);
  params.setUntrustedMem(utm_ptr, untrusted_size);
  params.setSimulated(true);
  
  EXPECT_EQ(enclave.init(TEST_EAPP, EYRIE_RT, params), KEYSTONE_SUCCESS);
  EXPECT_EQ(memcmp(enclave.getHash(), ref_hash, MD_SIZE), 0);
  EXPECT_EQ(enclave.destroy(), KEYSTONE_SUCCESS);
}

TEST(Keystone_Init, InvalidRT) {
  /* Tests enclave with a non-existant runtime
   *
   * */
  Keystone enclave;
  Params params;

  params.setFreeMemSize(untrusted_size);
  params.setUntrustedMem(utm_ptr, untrusted_size);
  params.setSimulated(true);

  EXPECT_EQ(enclave.init(TEST_EAPP, FAKE_RT, params), KEYSTONE_ERROR);
}

TEST(Keystone_Init, InvalidEAPP) {
  /* Tests enclave with a non-existant enclave app
   *
   * */
  Keystone enclave;
  Params params;

  params.setFreeMemSize(untrusted_size);
  params.setUntrustedMem(utm_ptr, untrusted_size);
  params.setSimulated(true);

  EXPECT_EQ(enclave.init(FAKE_EAPP, EYRIE_RT, params), KEYSTONE_ERROR);
}

TEST(Keystone_Run, RunTest) {
  /* Tests running the enclave after initialization
   *
   * */
  Keystone enclave;
  Params params;

  params.setFreeMemSize(untrusted_size);
  params.setUntrustedMem(utm_ptr, untrusted_size);
  params.setSimulated(true);

  EXPECT_EQ(enclave.init(TEST_EAPP, EYRIE_RT, params), KEYSTONE_SUCCESS);
  EXPECT_EQ(enclave.run(), KEYSTONE_SUCCESS);
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
