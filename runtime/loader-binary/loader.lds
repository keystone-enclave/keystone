OUTPUT_ARCH("riscv")

ENTRY( _start )

PAGE_SIZE = 0x1000; /* TODO: figure out the page size to use here */ 
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0100; 

SECTIONS
{
  .text :
  {
    PROVIDE( _start = . );
    *(._start);
    *(*);

    /* 
    * Only text section is copied into final loader binary 
    * Stack is added to text section as temporary stack space for loader
    */
    . = ALIGN(8);
    PROVIDE ( _sstack = . );
    *(._sstack);
    . = . + STACK_SIZE;
    . = ALIGN(8);
    PROVIDE ( _estack = . );
    *(._estack);
  }

}
