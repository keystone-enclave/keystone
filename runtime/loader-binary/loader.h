#include "elf.h"
#include "string.h"

// method definitions
// extern int hello(void * i, uintptr_t dram_base);
void initializeFreeList(uintptr_t freeMemBase, uintptr_t dramBase, size_t dramSize);
// void initializeFreeList(uintptr_t freeMemBase, uintptr_t dramBase, size_t dramSize, 
//     uintptr_t* freeList, uintptr_t* epmBase, uintptr_t* epmSize);
int loadElf(elf_t* elf);
uintptr_t satp_new(uintptr_t pa);
int load_runtime(uintptr_t dummy, uintptr_t dram_base, uintptr_t dram_size, uintptr_t runtime_base, 
      uintptr_t user_base, uintptr_t free_base, uintptr_t untrusted_ptr, uintptr_t untrusted_size);
