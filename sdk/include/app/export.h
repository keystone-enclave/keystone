#ifndef SDK_EXPORT_H
#define SDK_EXPORT_H

#include <stdint.h>
#include <stdlib.h>

struct export_function {
  char name[32];
  const uintptr_t addr;
  const uintptr_t base_va;

  size_t size;
  size_t scratch_size;
};

#endif  // SDK_EXPORT_H
