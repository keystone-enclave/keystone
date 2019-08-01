#ifndef _FU540_H_
#define _FU540_H_

#include "waymasks.h"

struct platform_enclave_data {

  size_t num_ways;
  waymask_t saved_mask;
  waymask_t scratchpad_mask;
  int scratch_rid;
};

#endif /* _FU540_H_ */
