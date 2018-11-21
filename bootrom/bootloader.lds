OUTPUT_ARCH( "riscv" )

ENTRY( _entry )

SECTIONS
{
  . = 0x2000; /* boot loader lives in boot ROM after the device tree */
  PROVIDE( reset_vector = . );
  .text :
  {
    PROVIDE( _entry = . );
    *(.reset)
    *(*)
  }
}

INCLUDE sanctum_params.lds
