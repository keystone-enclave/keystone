#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_

void edge_init();

unsigned long ocall_print_buffer(char* data, size_t data_len);
void ocall_print_value(unsigned long val);
#endif /* _EDGE_WRAPPER_H_ */
