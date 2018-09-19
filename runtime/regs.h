#define REGBYTES 64
#define STORE sd
#define LOAD ld
#define LWU lwu
#define LOG_REGBYTES  3
#define CONTEXT_SIZE (REGBYTES*32) 

#ifndef __ASSEMBLER__

#include "mm.h"

struct regs {
	unsigned long sepc;
	unsigned long ra;
	unsigned long sp;
	unsigned long gp;
	unsigned long tp;
	unsigned long t0;
	unsigned long t1;
	unsigned long t2;
	unsigned long s0;
	unsigned long s1;
	unsigned long a0;
	unsigned long a1;
	unsigned long a2;
	unsigned long a3;
	unsigned long a4;
	unsigned long a5;
	unsigned long a6;
	unsigned long a7;
	unsigned long s2;
	unsigned long s3;
	unsigned long s4;
	unsigned long s5;
	unsigned long s6;
	unsigned long s7;
	unsigned long s8;
	unsigned long s9;
	unsigned long s10;
	unsigned long s11;
	unsigned long t3;
	unsigned long t4;
	unsigned long t5;
	unsigned long t6;
	/* Supervisor CSRs */
	unsigned long sstatus;
	unsigned long sbadaddr;
	unsigned long scause;
  /* a0 value before the syscall */
  unsigned long orig_a0;
};
#endif
