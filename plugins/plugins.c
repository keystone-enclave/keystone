#include "plugins/plugins.h"

#ifdef PLUGIN_ENABLE_MULTIMEM
  #include "plugins/multimem.c"
#endif

uintptr_t
call_plugin(
    enclave_id id,
    uintptr_t plugin_id,
    uintptr_t call_id,
    uintptr_t arg0,
    uintptr_t arg1)
{
  switch(plugin_id) {
#ifdef PLUGIN_ENABLE_MULTIMEM
    case PLUGIN_ID_MULTIMEM:
      return do_sbi_multimem(plugin_id, call_id);
      break;
#endif
    default:
      // TOO fix it
      return -ENOSYS;
  }
}

