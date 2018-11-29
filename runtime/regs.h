//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#define STORE sd
#define LOAD ld
#define LWU lwu
#define LOG_REGBYTES  3
#define REGBYTES (1<<LOG_REGBYTES)
#define ENCL_CONTEXT_SIZE (REGBYTES*35)
#define HOST_CONTEXT_SIZE (REGBYTES*32)


#ifndef __ASSEMBLER__
#include <stdint.h>

struct regs_t {
	uintptr_t sepc; // use this slot as sepc
	uintptr_t ra;
	uintptr_t sp;
	uintptr_t gp;
	uintptr_t tp;
	uintptr_t t0;
	uintptr_t t1;
	uintptr_t t2;
	uintptr_t s0;
	uintptr_t s1;
	uintptr_t a0;
	uintptr_t a1;
	uintptr_t a2;
	uintptr_t a3;
	uintptr_t a4;
	uintptr_t a5;
	uintptr_t a6;
	uintptr_t a7;
	uintptr_t s2;
	uintptr_t s3;
	uintptr_t s4;
	uintptr_t s5;
	uintptr_t s6;
	uintptr_t s7;
	uintptr_t s8;
	uintptr_t s9;
	uintptr_t s10;
	uintptr_t s11;
	uintptr_t t3;
	uintptr_t t4;
	uintptr_t t5;
	uintptr_t t6;
};

struct encl_ctx_t {
	struct regs_t regs;
  /* Supervisor CSRs */
	uintptr_t sstatus;//32
	uintptr_t sbadaddr;//33
	uintptr_t scause;//34
};


#endif
