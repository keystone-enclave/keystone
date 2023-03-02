
#ifndef FUNCNAME
#error "FUNCNAME not defined"
#endif

#ifndef FNAME
#error "FNAME not defined"
#endif

#ifndef FBASE
#error "FBASE not defined"
#endif

#ifndef FSCRATCH
#error "FSCRATCH not defined"
#endif

#define MEMORY_DEFINES_ONLY
#include "host/Memory.hpp"
#undef MEMORY_DEFINES_ONLY

#include "app/export.h"

#define _APPEND(a, b) a ## b
#define _DEFER(a) a
#define _STRINGIFY(a) #a

#define ENTRYNAME(name) \
  _APPEND(name, _entry)

#define DATANAME(name) \
  _APPEND(name, _data)

#define FUNCSTRING(name) \
  _STRINGIFY(name)

__attribute__((aligned(RISCV_PGSIZE)))
static const uint8_t DATANAME(FUNCNAME)[] = {
#include FNAME
};

_Static_assert(sizeof(DATANAME(FUNCNAME)) % RISCV_PGSIZE == 0,
               "Exported function size not page aligned");

__attribute__((section ("keystone_exported")))
__attribute__((used))
static const struct export_function ENTRYNAME(FUNCNAME) = {
  .name = FUNCSTRING(FUNCNAME),
  .addr = (uintptr_t) DATANAME(FUNCNAME),
  .base_va = FBASE,
  .size = sizeof(DATANAME(FUNCNAME)),
  .scratch_size = FSCRATCH
};
