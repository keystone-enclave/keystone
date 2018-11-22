#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include "report.h"
#include "ed25519/ed25519.h"

using namespace json11;

std::string Report::BytesToHex(byte* bytes, size_t len)
{
  unsigned int i;
  std::string str;
	for(i=0; i<len; i+=1)
  {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << std::hex << (uintptr_t)bytes[i];
    str += ss.str();
  }
  return str;
}

void Report::HexToBytes(byte* bytes, size_t len, std::string hexstr)
{
  unsigned int i;
  for(i=0; i<len; i++)
  {
    int data = 0;
    std::stringstream ss;
    ss << hexstr.substr(i*2, 2);
    ss >> std::hex >> data;
    bytes[i] = (byte) data;
  }
}

void Report::fromJson(std::string jsonstr)
{
  std::string err;
  const auto json = Json::parse(jsonstr, err);

  std::string device_pubkey = json["device_pubkey"].string_value();
  HexToBytes(report.dev_public_key, PUBLIC_KEY_SIZE, device_pubkey);
  
  std::string sm_hash = json["security_monitor"]["hash"].string_value();
  HexToBytes(report.sm.hash, MDSIZE, sm_hash);
  std::string sm_pubkey = json["security_monitor"]["pubkey"].string_value();
  HexToBytes(report.sm.public_key, PUBLIC_KEY_SIZE, sm_pubkey);
  std::string sm_signature = json["security_monitor"]["signature"].string_value();
  HexToBytes(report.sm.signature, SIGNATURE_SIZE, sm_signature);

  std::string enclave_hash = json["enclave"]["hash"].string_value();
  HexToBytes(report.enclave.hash, MDSIZE, enclave_hash);
  report.enclave.data_len = json["enclave"]["datalen"].int_value();
  std::string enclave_data = json["enclave"]["data"].string_value();
  HexToBytes(report.enclave.data, report.enclave.data_len, enclave_data);
  std::string enclave_signature = json["enclave"]["signature"].string_value();
  HexToBytes(report.enclave.signature, SIGNATURE_SIZE, enclave_signature);
} 

void Report::fromBytes(byte* bin)
{
  std::memcpy(&report, bin, sizeof(struct report_t));
}

std::string Report::stringfy()
{
  if (report.enclave.data_len > ATTEST_DATA_MAXLEN)
  {
    return "{ \"error\" : \"invalid data length\" }" ;
  }
  auto json =
    Json::object {
      { "device_pubkey", BytesToHex(report.dev_public_key, PUBLIC_KEY_SIZE) },
      {
        "security_monitor", Json::object {
          { "hash", BytesToHex(report.sm.hash, MDSIZE)},
          { "pubkey", BytesToHex(report.sm.public_key, PUBLIC_KEY_SIZE)},
          { "signature", BytesToHex(report.sm.signature, SIGNATURE_SIZE)}
        },
      },
      {
        "enclave" , Json::object {
          { "hash", BytesToHex(report.enclave.hash, MDSIZE) },
          { "datalen", (int) report.enclave.data_len },
          { "data", BytesToHex(report.enclave.data, report.enclave.data_len) },
          { "signature", BytesToHex(report.enclave.signature, SIGNATURE_SIZE) },
        },
      },
    };
 
  return Json(json).dump();
}

void Report::printJson()
{
  std::cout << stringfy() << std::endl;
}

int Report::verify(void* data, size_t datalen)
{
  int sm_valid, enclave_valid = 1;
  /* verify SM report */
  sm_valid = ed25519_verify(report.sm.signature, (byte*) &report.sm, MDSIZE + PUBLIC_KEY_SIZE, report.dev_public_key);

  /* verify Enclave report */
  enclave_valid &= (report.enclave.data_len == datalen);
  enclave_valid &= (memcmp(report.enclave.data, data, datalen) == 0);
  enclave_valid &= ed25519_verify(report.enclave.signature, (byte*) &report.enclave,
      MDSIZE + sizeof(uint64_t) + datalen, report.sm.public_key);

  return sm_valid && enclave_valid;
}

int Report::verify(void)
{
  return verify(nullptr, 0);
}
