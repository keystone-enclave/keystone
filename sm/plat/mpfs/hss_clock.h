#ifndef HSS_CLOCK_H
#define HSS_CLOCK_H

/*******************************************************************************
 * Copyright 2019-2020 Microchip Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Hart Software Services - Clock and Timer Abstraction
 */

/**
 * \file Local Clock Abstraction
 * \brief Local Clock Abstraction methods - get tick count
 */

#ifdef __cplusplus
extern "C" {
#endif

//#  define TICKS_PER_SEC 50000llu // This is about 1 sec on RENODE
//#  define TICKS_PER_MILLISEC 5llu // This is about 1 millisec on RENODE

#if IS_ENABLED(CONFIG_PLATFORM_MPFS)
#  include "clocks/hw_mss_clks.h"
#  define TICKS_PER_SEC ((unsigned long long)LIBERO_SETTING_MSS_RTC_TOGGLE_CLK)
#  define TICKS_PER_MILLISEC    (TICKS_PER_SEC/1000llu)
#  define ONE_SEC (1llu * TICKS_PER_SEC)
#  define ONE_MILLISEC (1llu * TICKS_PER_MILLISEC)
#endif

#if IS_ENABLED(CONFIG_PLATFORM_FU540)
#  define TICKS_PER_MILLISEC    1000llu
#  define TICKS_PER_SEC         (1000llu * TICKS_PER_MILLISEC)
#  define ONE_SEC               (1llu * TICKS_PER_SEC)
#  define ONE_MILLISEC          (1llu * TICKS_PER_MILLISEC)
#endif

typedef uint64_t HSSTicks_t;
HSSTicks_t HSS_GetTime(void);
HSSTicks_t HSS_GetTickCount(void);
bool HSS_Timer_IsElapsed(HSSTicks_t startTick, HSSTicks_t durationInTicks);
void HSS_SpinDelay_MilliSecs(uint32_t milliseconds);
void HSS_SpinDelay_Secs(uint32_t seconds);

#ifdef __cplusplus
}
#endif

#endif
