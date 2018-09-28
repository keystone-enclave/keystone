OUTPUT_FORMAT("binary")

SECTIONS {
  . = 0x0000
  .text : { *(.text) }
  .data : { *(.data) }
  .bss : { *(.bss) }
}
