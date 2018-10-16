OUTPUT_ARCH( "riscv" )

SECTIONS 
{
  . = 0xa0000000;
  PROVIDE(rt_base = .);
  .text : { 
    *(.text._start)
    *(.text.encl_trap_handler)
    *(.text)
  }
  . = ALIGN(0x1000);
  .rodata : 
  { 
    *(.rdata)
    *(.rodata)
  }
  .data : { *(.data) }
  .bss : { *(.bss) }
  _end = .;
}
