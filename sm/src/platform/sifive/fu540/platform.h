#ifndef _FU540_H_
#define _FU540_H_

#include "waymasks.h"

struct platform_enclave_data {
  /* 0 means don't do cache partitioning. Otherwise the number of ways
     required. */
  size_t num_ways;
  /* Used if there is a waymask needed (>0 num_ways)*/
  waymask_t saved_mask;

  /* 0 for doesn't use scratchpad, 1 for does */
  int use_scratch;
};

#endif /* _FU540_H_ */
