//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_

void edge_init();

unsigned long ocall_print_buffer(char* data, size_t data_len);
void ocall_print_value(unsigned long val);

void ocall_copy_report(void* report, size_t len);
#endif /* _EDGE_WRAPPER_H_ */
