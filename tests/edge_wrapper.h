#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_

#include <edge_call.hpp>
#include "keystone.h"


typedef struct packaged_str{
  unsigned long str_offset;
  size_t len;
} packaged_str_t;
  

int edge_init(Keystone* enclave);

void print_buffer_wrapper(void* buffer, size_t buffer_len);
unsigned long print_buffer(char* str);

void print_value_wrapper(void* shared_buffer, size_t shared_buffer_size);
void print_value(unsigned long val);

void copy_report_wrapper(void* shared_buffer, size_t shared_buffer_size);
void copy_report(void* shared_buffer);

void get_host_string_wrapper(void* shared_buffer, size_t shared_buffer_size);
char* get_host_string();

#endif /* _EDGE_WRAPPER_H_ */
