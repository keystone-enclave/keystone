//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#define INTERRUPT_CAUSE_SOFTWARE  1
#define INTERRUPT_CAUSE_TIMER     5
#define INTERRUPT_CAUSE_EXTERNAL  9

void init_timer(void);

#endif
