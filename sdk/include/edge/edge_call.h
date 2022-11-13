//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef __EDGE_CALL_H_
#define __EDGE_CALL_H_

#include "edge_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Used by edge call wrappers to safely generate pointers into shared
   data structs */

int
edge_call_check_ptr_valid(uintptr_t ptr, size_t data_len);

void
edge_call_init_internals(uintptr_t buffer_start, size_t buffer_len);

int
edge_call_get_ptr_from_offset(
    edge_data_offset offset, size_t data_len, uintptr_t* ptr);

int
edge_call_get_offset_from_ptr(
    uintptr_t ptr, size_t data_len, edge_data_offset* offset);

int
edge_call_args_ptr(struct edge_call* edge_call, uintptr_t* ptr, size_t* size);
int
edge_call_ret_ptr(struct edge_call* edge_call, uintptr_t* ptr, size_t* size);
uintptr_t
edge_call_data_ptr();
int
edge_call_setup_call(struct edge_call* edge_call, void* ptr, size_t size);
int
edge_call_setup_ret(struct edge_call* edge_call, void* ptr, size_t size);
int
edge_call_setup_wrapped_ret(
    struct edge_call* edge_call, void* ptr, size_t size);

void
incoming_call_dispatch(void* buffer);

int
register_call(unsigned long call_id, edgecallwrapper func);

#ifdef __cplusplus
}
#endif

#endif /* __EDGE_CALL_H_ */
