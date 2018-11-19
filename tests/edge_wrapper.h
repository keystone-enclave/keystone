#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_

#include <edge_call.hpp>
#include "keystone.h"

typedef struct packaged_str{
  unsigned long str_offset;
  size_t len;
} packaged_str_t;

typedef unsigned char byte;

typedef struct report_t {
  byte sm_hash[64];
  byte sm_pubkey[32];
  byte sm_signature[64];
  byte encl_hash[64];
  uint64_t encl_data_len;
  byte encl_data[1024];
  byte encl_signature[64];
} report_t;

int edge_init(Keystone* enclave);

void print_buffer_wrapper(void* buffer);
unsigned long print_buffer(char* str);

void print_value_wrapper(void* buffer);
void print_value(unsigned long val);

void copy_report_wrapper(void* buffer);
void copy_report(void* shared_buffer);

void get_host_string_wrapper(void* buffer);
const char* get_host_string();

#endif /* _EDGE_WRAPPER_H_ */
