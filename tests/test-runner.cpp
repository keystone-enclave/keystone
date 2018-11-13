#include <iostream>
#include <cstdio>
#include "keystone.h"
#include "edge_wrapper.h"

typedef unsigned char byte;

char* longstr = "hellohellohellohellohellohellohellohellohellohello";

struct report_t {
  byte sm_hash[64];
  byte sm_pubkey[32];
  byte sm_signature[64];
  byte encl_hash[64];
  uint64_t encl_data_len;
  byte encl_data[1024];
  byte encl_signature[64];
};

unsigned long print_buffer(char* str){
  printf("Enclave said: %s",str);
  return strlen(str);
}

void print_value(unsigned long val){
  printf("Enclave said value: %u\n",val);
  return;
}

char* get_host_string(){
  return longstr;
}

static struct report_t report;

void print_hex(void* buffer, size_t len)
{
  int i;
  for(i = 0; i < len; i+=sizeof(uintptr_t))
  {
    printf("%.16lx ", *((uintptr_t*) (buffer + i)));
  }
  printf("\n");
}

void copy_report(void* buffer)
{
  memcpy(&report, buffer, sizeof(struct report_t));

  printf("====== Attestation Report ======\n");
  printf(" * sm hash:\n");
  print_hex(report.sm_hash, 64);
  printf(" * sm pubkey:\n");
  print_hex(report.sm_pubkey, 32);
  printf(" * sm signature:\n");
  print_hex(report.sm_signature, 64);
  printf(" * encl hash: \n");
  print_hex(report.encl_hash, 64);
  printf(" * encl data: \n");
  print_hex(report.encl_data, report.encl_data_len);
  printf(" * encl signature: \n");
  print_hex(report.encl_signature, 64);
  printf("=================================\n");

}

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    printf("Usage: %s <eapp> <runtime>\n", argv[0]);
    return 0;
  }
  Keystone enclave;
  keystone_status_t err;
  uintptr_t retval;

  enclave.init(argv[1], argv[2], 8192, 4096, 0x1000);

  edge_init(&enclave);

  enclave.run();

  return 0;
}

