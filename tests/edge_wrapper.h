#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_

#include <edge_call.hpp>
#include "keystone.h"

int edge_init(Keystone* enclave);

void print_buffer_wrapper(Keystone* enclave);
unsigned long print_buffer(char* str);


#endif /* _EDGE_WRAPPER_H_ */
