//******************************************************************************
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

const uint8_t ref_hash[MD_SIZE] = {
    0xfd, 0xd8, 0x49, 0xe1, 0x89, 0xf8, 0x38, 0x45, 0xa6, 0x22, 0x33,
    0x78, 0x02, 0x22, 0xb9, 0xdf, 0x31, 0x32, 0xb7, 0xc1, 0xaa, 0x10,
    0xce, 0xb2, 0xec, 0x55, 0x6e, 0x78, 0xb4, 0x4f, 0x6e, 0xf6, 0x11,
    0x95, 0x12, 0xb5, 0x54, 0xc7, 0x6b, 0xc8, 0x5a, 0xe9, 0x36, 0xef,
    0x5c, 0x14, 0x7f, 0x37, 0x86, 0x4c, 0x36, 0x7c, 0x8f, 0xff, 0xa2,
    0x0c, 0x67, 0x0f, 0x17, 0xd5, 0xe6, 0x1e, 0x7e, 0xe1};

size_t untrusted_size = 2 * 1024 * 1024;
size_t freemem_size   = 48 * 1024 * 1024;

using Keystone::Enclave;
using Keystone::Params;
using Keystone::Error;
using Keystone::ElfFile;
using Keystone::Memory;

TEST(Copy_Elf, Valid) {
  Enclave enclave; 
  Params params; 
	
  params.setFreeMemSize(untrusted_size);
  params.setUntrustedSize(untrusted_size);
  params.setSimulated(true);
  
  enclave.init(TEST_EAPP, EYRIE_RT, params);
  
  ElfFile runtimeElf = ElfFile(EYRIE_RT); 
  size_t rtFileSize = runtimeElf.getFileSize(); 
  char* rtElfPtr = (char *) runtimeElf.getPtr();
   
  ElfFile enclaveElf = ElfFile(TEST_EAPP); 
  size_t eappFileSize = enclaveElf.getFileSize();
  char* eappElfPtr = (char *) enclaveElf.getPtr();

  Memory* pMemory = enclave.getMemory(); 
  uintptr_t runtimeElfAddr = enclave.getRuntimeElfAddr(); 
  char rtCopiedElf[rtFileSize]; 
  char* copiedRtPtr = (char *) pMemory->readMem(runtimeElfAddr, rtFileSize);
  memcpy((void*) rtCopiedElf, copiedRtPtr, rtFileSize); 

  uintptr_t enclaveElfAddr = enclave.getEnclaveElfAddr(); 
  char eappCopiedElf[eappFileSize]; 
  char* copiedEappPtr = (char *) pMemory->readMem(enclaveElfAddr, eappFileSize);
  memcpy((void*) eappCopiedElf, copiedEappPtr, eappFileSize); 

  ASSERT_STREQ(rtElfPtr, rtCopiedElf);
  ASSERT_STREQ(eappElfPtr, eappCopiedElf);
}


int
main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
