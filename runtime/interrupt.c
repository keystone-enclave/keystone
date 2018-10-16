#include "regs.h"
#include "sbi.h"
#include "timex.h"

#define DEFAULT_CLOCK_DELAY 40000

void init_timer(void)
{
  sbi_set_timer(get_cycles64() + DEFAULT_CLOCK_DELAY);
}

void handle_interrupts(struct regs_t* encl_ctx)
{
  sbi_set_timer(get_cycles64() + DEFAULT_CLOCK_DELAY);
  return;
}
