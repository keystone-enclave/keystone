#include "loader/elf.h"
#include "sm_call.h"

resource_ptr_t* findIdentityResident(uintptr_t dram_base, const char* filename);
int loadElf(elf_t* elf, bool user);
