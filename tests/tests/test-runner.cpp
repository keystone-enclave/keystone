//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <getopt.h>
#include <iostream>
#include <cstdio>
#include "keystone.h"
#include "edge_wrapper.h"
#include "report.h"
#include "test_dev_key.h"
#include <thread>

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
  Report report;

  report.fromBytes((unsigned char*)buffer);

  if (report.checkSignaturesOnly(_sanctum_dev_public_key))
  {
    printf("Attestation report SIGNATURE is valid\n");
  }
  else
  {
    printf("Attestation report is invalid\n");
  }
}

void run_background(Keystone *enclave){
	enclave->run(); 
}

int main(int argc, char** argv)
{
  if(argc < 3 || argc > 8)
  {
    printf("Usage: %s <eapp> <runtime> [--utm-size SIZE(K)] [--freemem-size SIZE(K)] [--time] [--load-only] [--utm-ptr 0xPTR]\n", argv[0]);
    return 0;
  }


  int self_timing = 0;
  int load_only = 0;

  size_t untrusted_size = 2*1024*1024;
  size_t freemem_size = 48*1024*1024;
  uintptr_t utm_ptr = (uintptr_t)DEFAULT_UNTRUSTED_PTR;
  uintptr_t utm_ptr1 = (uintptr_t)DEFAULT_UNTRUSTED_PTR;


  static struct option long_options[] =
    {
      {"time",         no_argument,       &self_timing, 1},
      {"load-only",    no_argument,       &load_only, 1},
      {"utm-size",     required_argument, 0, 'u'},
      {"utm-ptr",      required_argument, 0, 'p'},
      {"freemem-size", required_argument, 0, 'f'},
      {0, 0, 0, 0}
    };


  char* eapp_file = argv[1];
  char* rt_file = argv[2];
  
  char* eapp_file_1 = argv[3];
  char* rt_file_1 = argv[4];


  int c;
  int opt_index = 3;
  while (1){

    c = getopt_long (argc, argv, "u:p:f:",
                     long_options, &opt_index);

    if (c == -1)
      break;

    switch (c){
    case 0:
      break;
    case 'u':
      untrusted_size = atoi(optarg)*1024;
      break;
    case 'p':
      utm_ptr = strtoll(optarg, NULL, 16);
      break;
    case 'f':
      freemem_size = atoi(optarg)*1024;
      break;
    }
  }

  Keystone enclave;
  Params params;

  Keystone enclave_1;
  Params params_1; 

  unsigned long cycles1,cycles2,cycles3,cycles4;

  params.setFreeMemSize(freemem_size);
  params.setUntrustedMem(utm_ptr, untrusted_size);

  params_1.setFreeMemSize(freemem_size);
  params_1.setUntrustedMem(utm_ptr1, untrusted_size);

  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles1));
  }

  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles2));
  }

  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles3));
  }

  int retcode = 0;
  if( !load_only ){
    
    if(!fork()){
      enclave_1.init(eapp_file_1, rt_file_1, params_1);
      edge_init(&enclave_1); 
      run_background(&enclave_1);
    } else {
      enclave.init(eapp_file, rt_file , params);
      edge_init(&enclave);
      run_background(&enclave);
    }

/*
    std::thread t1(run_background, &enclave_1);
    std::thread t2(run_background, &enclave);   
    t:x
1.join();
    t2.join(); 
*/  }

  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles4));
    printf("[keystone-test] Init: %lu cycles\r\n", cycles2-cycles1);
    printf("[keystone-test] Runtime: %lu cycles\r\n", cycles4-cycles3);
  }

  return retcode;
}
