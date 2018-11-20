#include <iostream>
#include <cstdio>
#include "keystone.h"
#include "edge_wrapper.h"

const char* longstr = "hellohellohellohellohellohellohellohellohellohello";

unsigned long print_buffer(char* str){
  printf("Enclave said: %s",str);
  return strlen(str);
}

void print_value(unsigned long val){
  printf("Enclave said value: %u\n",val);
  return;
}

const char* get_host_string(){
  return longstr;
}

static struct report_t report;

void print_hex(void* buffer, size_t len)
{
  int i;
  for(i = 0; i < len; i+=sizeof(uintptr_t))
  {
    printf("%.16lx ", *((uintptr_t*) ((uintptr_t)buffer + i)));
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
  Params params;

  params.setEnclaveEntry(0x1000);

  enclave.init(argv[1], argv[2], params);

  edge_init(&enclave);

  enclave.run();

  return 0;
}

