#include <edge_call.h>

int edge_call_get_ptr_from_offset(uintptr_t shared_start, size_t shared_len,
				  edge_data_offset offset, size_t data_len,
				  uintptr_t* ptr){

  //TODO double check these checks
  
  /* Validate that shared_start+offset is sane */
  if( offset > UINTPTR_MAX - shared_start ||
      offset > shared_len ){
    return -1;
  }
  
  
  /* Validate that shared_start+offset+data_len in range */
  if( data_len > UINTPTR_MAX - (shared_start+offset) ||
      data_len > shared_len - offset ){
    return -1;
  }

  /* ptr looks valid, create it */
  *ptr = shared_start+offset;
  return 0;
}
