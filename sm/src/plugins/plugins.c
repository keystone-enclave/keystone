#include "plugins.h"

#ifdef PLUGIN_ENABLE_MULTIMEM
  #include "multimem.c"
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
      return do_sbi_multimem(id, call_id, arg0);
      break;
#endif
    default:
      // TOO fix it
      return SBI_ERR_SM_NOT_IMPLEMENTED;
  }
}

