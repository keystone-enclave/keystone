OUTPUT_ARCH( "riscv" )

SECTIONS 
{
  . = 0x0;
  _start = .;
  .text : { *(.text) }
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
