#include "edge_call.h"

edgecallwrapper edge_call_table[MAX_EDGE_CALL];

/* Registered handler for incoming edge calls */
void incoming_call_dispatch(void* shared_buffer, size_t buffer_size){
  struct edge_call_t* edge_call = (struct edge_call_t*) shared_buffer;

  /* Lookup the call in the table */
  if( edge_call->call_id > MAX_EDGE_CALL ||
      edge_call_table[edge_call->call_id] == NULL ){
    /* Fatal error */
    goto fatal_error;
  }

  edge_call_table[edge_call->call_id](shared_buffer, buffer_size);
  return;


  fatal_error:
    edge_call->return_data.call_status = CALL_STATUS_BAD_CALL_ID;
    return;
}

int register_call(unsigned long call_id, edgecallwrapper func){
  if( call_id > MAX_EDGE_CALL){
    return -1;
  }
  edge_call_table[call_id] = func;
  return 0;
}
