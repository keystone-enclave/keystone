#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"

void EAPP_ENTRY eapp_entry(){

  char* buffer = (char*)untrusted_mmap();
  memcpy(buffer, "hello world!\n", 14);
  ocall(1);

  EAPP_RETURN((unsigned long)buffer);
}
