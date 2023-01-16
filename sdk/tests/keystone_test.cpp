//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------

#include <getopt.h>
#include <keystone.h>

#include <cstdio>
#include <iostream>
#include <string>
#include <thread>

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

TEST(LoggingTest, RedirectsWithoutError) {
  /* Tests if we can direct to the standard streams or files without error. */
  Keystone::Logger logger{};
  EXPECT_TRUE(logger.DirectToSTDOUT());
  EXPECT_TRUE(logger.DirectToSTDERR());
  EXPECT_TRUE(logger.DirectToFile("a.txt"));
  EXPECT_TRUE(logger.DirectToSTDOUT());  // back to a standard stream
}

TEST(LoggingTest, RedirectsToFile) {
  /* Tests if logs are written to a file after directing to it. */
  constexpr const char* file_name = "log.txt";
  std::cout << "Started\n";

  {
    Keystone::Logger log{};
    EXPECT_TRUE(log.DirectToFile(file_name));
    log << "Hello, here's a number: " << 17;
  }

  std::ifstream fin{file_name};
  std::string buf{};
  std::getline(fin, buf);
  EXPECT_EQ("Hello, here's a number: 17", buf);
  fin.close();
}

TEST(LoggingTest, RedirectsAwayFromFile) {
  /* Tests if we stop writing to a file after directing away from it. */
  constexpr const char* file_name = "log.txt";

  {
    Keystone::Logger log{};

    EXPECT_TRUE(log.DirectToFile(file_name));
    log << "[file]";

    EXPECT_TRUE(log.DirectToSTDOUT());
    log << "[cout]";

    EXPECT_TRUE(log.DirectToSTDERR());
    log << "[cerr]";
  }

  std::ifstream fin{file_name};
  std::string buf{};
  std::getline(fin, buf);
  EXPECT_EQ("[file]", buf);
}

TEST(LoggingTest, RespondsToDisableEnable) {
  /* Tests if the log starts/stops writing in response to enable/disable. */
  constexpr const char* file_name = "log.txt";

  {
    Keystone::Logger log{};
    EXPECT_TRUE(log.DirectToFile(file_name));
    log << "a";
    log.Disable();
    log << "b";
    log.Enable();
    log << "c";
  }

  std::ifstream fin{file_name};
  std::string buf{};
  std::getline(fin, buf);
  EXPECT_EQ("ac", buf);
}

TEST(LoggingTest, AppendsFile) {
  /* Tests if the log correctly appends the file if the append option
     is selected. */
  constexpr const char* file_name = "log.txt";

  {
    std::ofstream fout{file_name};
    fout << "logs:";
    fout.flush();
  }

  {
    Keystone::Logger log{};
    EXPECT_TRUE(log.DirectToFile(file_name, true));
    log << "my log";
  }

  std::ifstream fin{file_name};
  std::string buf{};
  std::getline(fin, buf);
  EXPECT_EQ("logs:my log", buf);
}

TEST(LoggingTest, ConcurrentWrites) {
  /* Tests if we write to the same log from two threads without error. */

  /* We pick a large number to increase the chance of overlap in
     scheduling between the two threads. */
  constexpr auto n_chars          = 1000000;
  constexpr auto c                = 'x';
  constexpr const char* file_name = "log.txt";
  {
    Keystone::Logger log{};
    EXPECT_TRUE(log.DirectToFile(file_name));
    auto log_half = [&]() {
      for (auto half = n_chars / 2; half; --half) {
        log << c;
      }
    };
    std::thread t1{log_half};
    std::thread t2{log_half};

    t1.join();
    t2.join();
  }

  std::ifstream fin{file_name};
  EXPECT_TRUE(fin.is_open());
  std::string buf{};
  std::getline(fin, buf);
  EXPECT_EQ(std::string(n_chars, c), buf);
}

int
main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
