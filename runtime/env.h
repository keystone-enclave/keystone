#ifndef __ENV_H__
#define __ENV_H__

#include "rt_elf.h"

void* setup_start(void* _sp, ELF(Ehdr) *hdr);

#endif
