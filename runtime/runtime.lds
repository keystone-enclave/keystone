OUTPUT_ARCH( "riscv" )

SECTIONS 
{
  . = 0x0;
  .text : { 
    *(.text.trampoline)
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
