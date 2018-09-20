#include "bits.h"

save_host_regs:
  .global save_host_regs
  STORE x1, 1*REGBYTES(tp)
  STORE x3, 3*REGBYTES(tp)
  //STORE x4, 4*REGBYTES(tp)
  STORE x5, 5*REGBYTES(tp)
  STORE x6, 6*REGBYTES(tp)
  STORE x7, 7*REGBYTES(tp)
  STORE x8, 8*REGBYTES(tp)
  STORE x9, 9*REGBYTES(tp)
  STORE x10, 10*REGBYTES(tp)
  STORE x11, 11*REGBYTES(tp)
  STORE x12, 12*REGBYTES(tp)
  STORE x13, 13*REGBYTES(tp)
  STORE x14, 14*REGBYTES(tp)
  STORE x15, 15*REGBYTES(tp)
  STORE x16, 16*REGBYTES(tp)
  STORE x17, 17*REGBYTES(tp)
  STORE x18, 18*REGBYTES(tp)
  STORE x19, 19*REGBYTES(tp)
  STORE x20, 20*REGBYTES(tp)
  STORE x21, 21*REGBYTES(tp)
  STORE x22, 22*REGBYTES(tp)
  STORE x23, 23*REGBYTES(tp)
  STORE x24, 24*REGBYTES(tp)
  STORE x25, 25*REGBYTES(tp)
  STORE x26, 26*REGBYTES(tp)
  STORE x27, 27*REGBYTES(tp)
  STORE x28, 28*REGBYTES(tp)
  STORE x29, 29*REGBYTES(tp)
  STORE x30, 30*REGBYTES(tp)
  STORE x31, 31*REGBYTES(tp)
  STORE x4, 4*REGBYTES(tp)
  STORE sp, 2*REGBYTES(tp)
  ret

clean_regs:
  li x1, 0
  li x2, 0
  li x3, 0
  li x4, 0
  li x5, 0
  li x6, 0
  li x7, 0
  li x8, 0
  li x9, 0
  li x10, 0
  li x11, 0
  li x12, 0
  li x13, 0
  li x14, 0
  li x15, 0
  li x16, 0
  li x17, 0
  li x18, 0
  li x19, 0
  li x20, 0
  li x21, 0
  li x22, 0
  li x23, 0
  li x24, 0
  li x25, 0
  li x26, 0
  li x27, 0
  li x28, 0
  li x29, 0
  li x30, 0
  li x31, 0
