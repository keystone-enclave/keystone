#pragma once

#include <stdint.h>

void
pswap_init(void);

void
page_swap_epm(uintptr_t back_page, uintptr_t epm_page, uintptr_t swap_page);
