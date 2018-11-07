#ifndef __EDGE_COMMON_H_
#define _EDGE_COMMON_H_
#include <stdint.h>
#include <stddef.h>

/* We want to handle everything in terms of shared data region offsets
   to minimize vaddr problems. We'd have to do the translation anyway,
   so we make it explicit */
typedef size_t edge_data_offset;


/* Call status indicates if the wrapper code, pointers, offsets, etc went OK
 * It has no bearing on data contained in the returns. */
#define CALL_STATUS_OK 0
#define CALL_STATUS_BAD_CALL_ID 1
#define CALL_STATUS_BAD_OFFSET 2
#define CALL_STATUS_BAD_PTR 3
#define CALL_STATUS_ERROR 4


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


#endif /* __EDGE_COMMON_H_ */
