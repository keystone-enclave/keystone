//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <edge_call.h>
#include "string.h"
#include <sys/epoll.h>
#include <sys/socket.h>

uintptr_t _shared_start;
size_t _shared_len;

void
edge_call_init_internals(uintptr_t buffer_start, size_t buffer_len) {
  _shared_start = buffer_start;
  _shared_len   = buffer_len;
}

int
edge_call_get_ptr_from_offset(
    edge_data_offset offset, size_t data_len, uintptr_t* ptr) {
  // TODO double check these checks

  /* Validate that _shared_start+offset is sane */
  if (offset > UINTPTR_MAX - _shared_start || offset > _shared_len) {
    return -1;
  }

  /* Validate that _shared_start+offset+data_len in range */
  if (data_len > UINTPTR_MAX - (_shared_start + offset) ||
      data_len > _shared_len - offset) {
    return -1;
  }

  /* ptr looks valid, create it */
  *ptr = _shared_start + offset;
  return 0;
}

int
edge_call_check_ptr_valid(uintptr_t ptr, size_t data_len) {
  // TODO double check these checks

  /* Validate that ptr starts in range */
  if (ptr > _shared_start + _shared_len || ptr < _shared_start) {
    return 1;
  }

  if (data_len > UINTPTR_MAX - ptr) {
    return 2;
  }

  /* Validate that the end is in range */
  if (ptr + data_len > _shared_start + _shared_len) {
    return 3;
  }

  return 0;
}

int
edge_call_get_offset_from_ptr(
    uintptr_t ptr, size_t data_len, edge_data_offset* offset) {
  int valid = edge_call_check_ptr_valid(ptr, data_len);
  if (valid != 0) return valid;

  /* ptr looks valid, create it */
  *offset = ptr - _shared_start;
  return 0;
}

int
edge_call_args_ptr(struct edge_call* edge_call, uintptr_t* ptr, size_t* size) {
  *size = edge_call->call_arg_size;
  return edge_call_get_ptr_from_offset(edge_call->call_arg_offset, *size, ptr);
}

int
edge_call_ret_ptr(struct edge_call* edge_call, uintptr_t* ptr, size_t* size) {
  *size = edge_call->return_data.call_ret_size;
  return edge_call_get_ptr_from_offset(
      edge_call->return_data.call_ret_offset, *size, ptr);
}

int
edge_call_setup_call(struct edge_call* edge_call, void* ptr, size_t size) {
  edge_call->call_arg_size = size;
  return edge_call_get_offset_from_ptr(
      (uintptr_t)ptr, size, &edge_call->call_arg_offset);
}

int
edge_call_setup_ret(struct edge_call* edge_call, void* ptr, size_t size) {
  edge_call->return_data.call_ret_size = size;
  return edge_call_get_offset_from_ptr(
      (uintptr_t)ptr, size, &edge_call->return_data.call_ret_offset);
}

/* This is only usable for the host */
int
edge_call_setup_wrapped_ret(
    struct edge_call* edge_call, void* ptr, size_t size) {
  struct edge_data data_wrapper;
  data_wrapper.size = size;
  edge_call_get_offset_from_ptr(
      _shared_start + sizeof(struct edge_call) + sizeof(struct edge_data),
      sizeof(struct edge_data), &data_wrapper.offset);

  memcpy(
      (void*)(_shared_start + sizeof(struct edge_call) + sizeof(struct edge_data)),
      ptr, size);

  memcpy(
      (void*)(_shared_start + sizeof(struct edge_call)), &data_wrapper,
      sizeof(struct edge_data));

  edge_call->return_data.call_ret_size = sizeof(struct edge_data);
  return edge_call_get_offset_from_ptr(
      _shared_start + sizeof(struct edge_call), sizeof(struct edge_data),
      &edge_call->return_data.call_ret_offset);
}

/* This is temporary until we have a better way to handle multiple things */
uintptr_t
edge_call_data_ptr() {
  return _shared_start + sizeof(struct edge_call);
}
