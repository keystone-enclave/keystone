OUTPUT_ARCH( "riscv" )

ENTRY( _entry )

SECTIONS
{
  . = 0x1000; /* boot loader lives in boot ROM after the device tree */
  PROVIDE( reset_vector = . );
  .text :
  {
    PROVIDE( _entry = . );
    *(.reset)
    *(*)
  }
  . = ALIGN(4);
  PROVIDE( _dtb = . );
}

INCLUDE sanctum_params.lds
