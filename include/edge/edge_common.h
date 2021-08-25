//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef __EDGE_COMMON_H_
#define __EDGE_COMMON_H_
#include <stddef.h>
#include <stdint.h>

/* We want to handle everything in terms of shared data region offsets
   to minimize vaddr problems. We'd have to do the translation anyway,
   so we make it explicit */
typedef size_t edge_data_offset;

typedef void (*edgecallwrapper)(void*);

#define MAX_EDGE_CALL 10

extern edgecallwrapper edge_call_table[MAX_EDGE_CALL];

/* Call status indicates if the wrapper code, pointers, offsets, etc went OK
 * It has no bearing on data contained in the returns. */
#define CALL_STATUS_OK 0
#define CALL_STATUS_BAD_CALL_ID 1
#define CALL_STATUS_BAD_OFFSET 2
#define CALL_STATUS_BAD_PTR 3
#define CALL_STATUS_ERROR 4
#define CALL_STATUS_SYSCALL_FAILED 5

/* Must init these */
extern uintptr_t _shared_start;
extern size_t _shared_len;

/* Useful type for things like packaged strings, etc */
struct edge_data {
  edge_data_offset offset;
  size_t size;
};

struct edge_app_retdata {
  void* app_ptr;
  size_t len;
};

struct edge_return {
  /* Status variable indicating error/success conditions. Not for data
     values. */
  unsigned long call_status;

  /* OFFSET into the shared memory region.  Should be checked for
   * validity, then turned into a pointer to a relevant return data
   * structure for the call. (User/call defined) */
  edge_data_offset call_ret_offset;
  size_t call_ret_size;
};

struct edge_call {
  /* Similar to syscall number.  User-defined call id, handled at the
   * edges only */
  unsigned long call_id;

  /* OFFSET into the shared memory region.  Should be checked for
   * validity, then turned into a pointer to a relevant argument
   * structure for the call. (User/call defined)*/
  edge_data_offset call_arg_offset;
  size_t call_arg_size;

  /* Pre-set location to structure return data */
  struct edge_return return_data;
};

#endif /* __EDGE_COMMON_H_ */
