#ifndef __EDGE_CALL_H_
#define __EDGE_CALL_H__

#include <stdint.h>
#include <stddef.h>

/* We want to handle everything in terms of shared data region offsets
   to minimize vaddr problems. We'd have to do the translation anyway,
   so we make it explicit */
typedef size_t edge_data_offset;

struct edge_return_t{

  /* Status variable indicating error/success conditions. Not for data
     values. */
  unsigned long call_status;

  /* OFFSET into the shared memory region.  Should be checked for
   * validity, then turned into a pointer to a relevant return data
   * structure for the call. (User/call defined) */
  edge_data_offset call_ret_offset;
};

struct edge_call_t{
  /* Similar to syscall number.  User-defined call id, handled at the
   * edges only */
  unsigned long call_id;

  /* OFFSET into the shared memory region.  Should be checked for
   * validity, then turned into a pointer to a relevant argument
   * structure for the call. (User/call defined)*/
  edge_data_offset call_arg_offset;

  /* Pre-set location to structure return data */
  struct edge_return_t return_data;
};


/* Used by edge call wrappers to safely generate pointers into shared
   data structs */
int edge_call_get_ptr_from_offset(uintptr_t shared_start, size_t shared_len,
				  edge_data_offset offset, size_t data_len,
				  uintptr_t* ptr);

#endif /* __EDGE_CALL_H_ */
