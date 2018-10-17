OUTPUT_ARCH( "riscv" )

SECTIONS 
{
  . = 0xa0000000;
  .text : { 
    *(.text._start)
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
