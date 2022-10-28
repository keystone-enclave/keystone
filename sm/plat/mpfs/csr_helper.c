/*******************************************************************************
 * Copyright 2019 Microchip Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HSS Embedded Software
 *
 */

/**
 * \file CSR Helper
 * \brief CSR Helper
 */

#include "config.h"
#include "csr_helper.h"
#include "mpfs_reg_map.h"


#include <assert.h>

HSSTicks_t CSR_GetTickCount(void)
{
    HSSTicks_t tickCount;

    tickCount = mHSS_CSR_READ(mcycle);

    return tickCount;
}

HSSTicks_t CSR_GetTime(void)
{
    HSSTicks_t time;

    time = mHSS_ReadRegU64(CLINT, MTIME);

    return time;
}
