//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <edge_call.h>
#include "string.h"

uintptr_t _shared_start;
size_t _shared_len;

void edge_call_init_internals(uintptr_t buffer_start, size_t buffer_len){
  _shared_start = buffer_start;
  _shared_len = buffer_len;
}

int edge_call_get_ptr_from_offset(edge_data_offset offset, size_t data_len,
				  uintptr_t* ptr){

  //TODO double check these checks
  
  /* Validate that _shared_start+offset is sane */
  if( offset > UINTPTR_MAX - _shared_start ||
      offset > _shared_len ){
    return -1;
  }
  
  
  /* Validate that _shared_start+offset+data_len in range */
  if( data_len > UINTPTR_MAX - (_shared_start+offset) ||
      data_len > _shared_len - offset ){
    return -1;
  }

  /* ptr looks valid, create it */
  *ptr = _shared_start+offset;
  return 0;
}


int edge_call_get_offset_from_ptr(uintptr_t ptr, size_t data_len,
				  edge_data_offset* offset){

  //TODO double check these checks
  
  /* Validate that ptr starts in range */
  if( ptr > _shared_start+_shared_len ||
      ptr < _shared_start ){
    return 1;
  }

  if( data_len > UINTPTR_MAX - ptr){
    return 2;
  }
  
  /* Validate that the end is in range */
  if( ptr+data_len  > _shared_start+_shared_len){
    return 3;
  }

  /* ptr looks valid, create it */
  *offset = ptr-_shared_start;
  return 0;
}


int edge_call_args_ptr(edge_call_t* edge_call, uintptr_t* ptr){

  return edge_call_get_ptr_from_offset(edge_call->call_arg_offset,
				       edge_call->call_arg_size, ptr);
}

int edge_call_ret_ptr(edge_call_t* edge_call, uintptr_t* ptr){
  return edge_call_get_ptr_from_offset(edge_call->return_data.call_ret_offset,
				       edge_call->return_data.call_ret_size, ptr);
}

int edge_call_setup_call(edge_call_t* edge_call, void* ptr, size_t size){
  edge_call->call_arg_size = size;
  return edge_call_get_offset_from_ptr((uintptr_t)ptr, size,
				       &edge_call->call_arg_offset);
}

int edge_call_setup_ret(edge_call_t* edge_call, void* ptr, size_t size){
  edge_call->return_data.call_ret_size = size;
  return edge_call_get_offset_from_ptr((uintptr_t)ptr, size,
				       &edge_call->return_data.call_ret_offset);
}

/* This is only usable for the host */
int edge_call_setup_wrapped_ret(edge_call_t* edge_call, void* ptr, size_t size){
  edge_data_t data_wrapper;
  data_wrapper.size = size;
  edge_call_get_offset_from_ptr(_shared_start+sizeof(edge_call_t)+sizeof(edge_data_t),
				sizeof(edge_data_t),
				&data_wrapper.offset);

  memcpy((void*)(_shared_start+sizeof(edge_call_t)+sizeof(edge_data_t)),
	 ptr,
	 size);

  memcpy((void*)(_shared_start+sizeof(edge_call_t)),
	 &data_wrapper,
	 sizeof(edge_data_t));
  
  edge_call->return_data.call_ret_size = sizeof(edge_data_t);
  return edge_call_get_offset_from_ptr(_shared_start+sizeof(edge_call_t),
				       sizeof(edge_data_t),
				       &edge_call->return_data.call_ret_offset);
}


/* This is temporary until we have a better way to handle multiple things */
uintptr_t edge_call_data_ptr(){
  return _shared_start + sizeof(edge_call_t);
}
