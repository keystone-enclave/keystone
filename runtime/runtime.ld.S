#ifdef USE_CALLEE
#include "call/callee.h"
#endif

#include "mm/vm_defs.h"

OUTPUT_ARCH( "riscv" )

SECTIONS
{
  . = 0xffffffffc0000000;
  PROVIDE(rt_base = .);
  .text : {
    *(.text._start)
    *(.text.encl_trap_handler)
    *(.text)
  }
  . = ALIGN(RISCV_PAGE_SIZE);
  .rodata :
  {
    *(.rdata)
    *(.rodata)
  }
  .data : { *(.data) }
  .bss : { *(.bss) }
  . = ALIGN(RISCV_PAGE_SIZE);
  .kernel_stack : {
    . += 8 * RISCV_PAGE_SIZE;
    PROVIDE(kernel_stack_end = .);
  }

#ifdef USE_DRIVERS
  .drivers : {
    PROVIDE(drivers_start = .);
    *(.drivers)
    PROVIDE(drivers_end = .);
  }
#endif

#ifdef USE_CALLEE
  .callee_stack : {
    . += 0x1000;
    PROVIDE(callee_stack_end = .);
  }

  .callee_kstacks : {
    PROVIDE(callee_kstacks_start = .);
    . += NUM_THREAD_SLOTS * RISCV_PAGE_SIZE;
    PROVIDE(callee_kstacks_end = .);
  }
#endif // USE_CALLEE

  _end = .;
}
