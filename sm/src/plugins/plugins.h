#ifndef __SM_PLUGINS_H__
#define __SM_PLUGINS_H__

#include "../sm.h"
#include "../enclave.h"

/* PLUGIN IDs */
#define PLUGIN_ID_MULTIMEM  0x1

#ifdef PLUGIN_ENABLE_MULTIMEM
  #include "plugins/multimem.h"
#endif

uintptr_t
call_plugin(
    enclave_id id,
    uintptr_t plugin_id,
    uintptr_t call_id,
    uintptr_t arg0,
    uintptr_t arg1
    );

#endif
