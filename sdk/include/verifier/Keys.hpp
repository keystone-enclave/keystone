//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

#include <string>

typedef unsigned char byte;

#define ATTEST_DATA_MAXLEN 1024
#define MDSIZE 64
#define SIGNATURE_SIZE 64
#define PUBLIC_KEY_SIZE 32

class PublicKey {
 public:
  byte data[PUBLIC_KEY_SIZE];
  explicit PublicKey(std::string hexstr);
};

class DevicePublicKey : public PublicKey {
 public:
  explicit DevicePublicKey(std::string hexstr) : PublicKey(hexstr) {}
};

class SecurityMonitorPublicKey : public PublicKey {
 public:
  explicit SecurityMonitorPublicKey(std::string hexstr) : PublicKey(hexstr) {}
};
