#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_

void edge_init();

typedef struct packaged_str{
  unsigned long str_offset;
  size_t len;
} packaged_str_t;

unsigned long ocall_print_buffer(char* data, size_t data_len);
void ocall_print_value(unsigned long val);
packaged_str_t ocall_get_string();
#endif /* _EDGE_WRAPPER_H_ */
