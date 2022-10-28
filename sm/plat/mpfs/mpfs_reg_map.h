#ifndef HSS_MPFS_REG_MAP_H
#define HSS_MPFS_REG_MAP_H

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
 *
 * Hart Software Services - Register Definitions
 *
 */


#ifdef __cplusplus
extern "C" {
#endif

#define E51_DTIM_BASE_ADDR	 	  	(0x01000000u)

#define E51_ITIM_BASE_ADDR	   		(0x01800000u)

#define U54_1_ITIM_BASE_ADDR	 		(0x01808000u)

#define U54_2_ITIM_BASE_ADDR	 		(0x01810000u)

#define U54_3_ITIM_BASE_ADDR	 		(0x01818000u)

#define U54_4_ITIM_BASE_ADDR	 		(0x01820000u)

#define CLINT_BASE_ADDR		  		(0x02000000u)
#define CLINT_MSIP_E51_0_OFFSET			(0x0000u)
#define CLINT_MSIP_U54_1_OFFSET			(0x0004u)
#define CLINT_MSIP_U54_2_OFFSET			(0x0008u)
#define CLINT_MSIP_U54_3_OFFSET			(0x000Cu)
#define CLINT_MSIP_U54_4_OFFSET			(0x0010u)
#define CLINT_MTIME_OFFSET		        (0xBFF8u)

#define L2_CACHE_CTRL_BASE_ADDR	  		(0x02010000u)
#define L2_CACHE_CTRL_CONFIG_OFFSET  		(0x000u)
#define L2_CACHE_CTRL_WAYENABLE_OFFSET  	(0x008u)
#define L2_CACHE_CTRL_WAYMASK0_OFFSET   	(0x800u)
#define L2_CACHE_CTRL_WAYMASK1_OFFSET   	(0x008u)
#define L2_CACHE_CTRL_WAYMASK2_OFFSET   	(0x810u)
#define L2_CACHE_CTRL_WAYMASK3_OFFSET   	(0x818u)
#define L2_CACHE_CTRL_WAYMASK4_OFFSET   	(0x820u)

#define WCB_BASE_ADDR				(0x02020000u)

#define DMA_CTRL_BASE_ADDR		   	(0x03000000u)

#define L2_LIM_BASE_ADDR			(0x08000000u)

#define L2_ZERODEV_BASE_ADDR		 	(0x0A000000u)

#ifndef PLIC_BASE_ADDR
#	define PLIC_BASE_ADDR			(0x0C000000u)
#endif

#define SYSREGSCB_BASE_ADDR			(0x20003000u)
#define SYSREGSCB_MSS_STATUS_OFFSET		(0x0104u)

#define MPU_BASE_ADDR				(0x2000E000u)
#define MPU_FIC0_OFFSET				(0x0000u)
#define MPU_FIC1_OFFSET				(0x0100u)
#define MPU_FIC2_OFFSET				(0x0200u)
#define MPU_CRYPTO_OFFSET			(0x0300u)
#define MPU_ETHERNET0_OFFSET			(0x0400u)
#define MPU_ETHERNET1_OFFSET			(0x0500u)
#define MPU_USB_OFFSET				(0x0600u)
#define MPU_MMC_OFFSET				(0x0700u)
#define MPU_SCB_OFFSET				(0x0800u)
#define MPU_SEG0_OFFSET				(0x0D00u)
#define MPU_SEG1_OFFSET				(0x0E00u)

#define WDOG0_LO_BASE_ADDR                      (0x20001000u)
#define WDOG1_LO_BASE_ADDR                      (0x20101000u)
#define WDOG2_LO_BASE_ADDR                      (0x20103000u)
#define WDOG3_LO_BASE_ADDR                      (0x20105000u)
#define WDOG4_LO_BASE_ADDR                      (0x20107000u)

#define WDOG_REFRESH_OFFSET                     (0x0000u)
#define WDOG_CONTROL_OFFSET                     (0x0004u)
#define WDOG_STATUS_OFFSET                      (0x0008u)
#define WDOG_TIME_OFFSET                        (0x000Cu)
#define WDOG_MSVP_OFFSET                        (0x0010u)
#define WDOG_TRIGGER_OFFSET                     (0x0014u)
#define WDOG_FORCE_OFFSET                       (0x0018u)

#define WDOG_STATUS_DEVRST_MASK                 (1u << 5)
#define WDOG_STATUS_LOCKED_MASK                 (1u << 4)
#define WDOG_STATUS_TRIGGERED_MASK              (1u << 3)
#define WDOG_STATUS_FORBIDDEN_MASK              (1u << 2)
#define WDOG_STATUS_WDOG_TRIPPED_MASK           (1u << 1)
#define WDOG_STATUS_MVSP_TRIPPED_MASK           (1u << 0)

#define WDOG0_LO_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG0_LO_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG0_LO_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG0_LO_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG0_LO_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG0_LO_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG0_LO_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define WDOG1_LO_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG1_LO_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG1_LO_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG1_LO_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG1_LO_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG1_LO_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG1_LO_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define WDOG2_LO_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG2_LO_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG2_LO_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG2_LO_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG2_LO_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG2_LO_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG2_LO_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define WDOG3_LO_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG3_LO_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG3_LO_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG3_LO_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG3_LO_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG3_LO_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG3_LO_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define WDOG4_LO_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG4_LO_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG4_LO_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG4_LO_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG4_LO_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG4_LO_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG4_LO_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define WDOG0_HI_BASE_ADDR                      (0x28001000u)
#define WDOG1_HI_BASE_ADDR                      (0x28101000u)
#define WDOG2_HI_BASE_ADDR                      (0x28103000u)
#define WDOG3_HI_BASE_ADDR                      (0x28105000u)
#define WDOG4_HI_BASE_ADDR                      (0x28107000u)

#define WDOG0_HI_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG0_HI_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG0_HI_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG0_HI_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG0_HI_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG0_HI_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG0_HI_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define WDOG1_HI_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG1_HI_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG1_HI_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG1_HI_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG1_HI_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG1_HI_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG1_HI_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define WDOG2_HI_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG2_LO_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG2_HI_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG2_HI_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG2_LO_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG2_HI_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG2_LO_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define WDOG3_LO_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG3_HI_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG3_HI_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG3_LO_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG3_HI_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG3_HI_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG3_HI_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define WDOG4_HI_REFRESH_OFFSET WDOG_REFRESH_OFFSET
#define WDOG4_HI_CONTROL_OFFSET WDOG_CONTROL_OFFSET
#define WDOG4_HI_STATUS_OFFSET  WDOG_STATUS_OFFSET
#define WDOG4_HI_TIME_OFFSET    WDOG_TIME_OFFSET
#define WDOG4_HI_MSVP_OFFSET    WDOG_MSVP_OFFSET
#define WDOG4_HI_TRIGGER_OFFSET WDOG_TRIGGER_OFFSET
#define WDOG4_LO_FORCE_OFFSET   WDOG_FORCE_OFFSET

#define mHSS_WriteRegEx(type, block, reg, value) { *(type*)(block##_BASE_ADDR + block##_##reg##_OFFSET) = value; }
#define mHSS_WriteRegU8(block, reg, value)  mHSS_WriteRegEx(uint8_t, block, reg, value)
#define mHSS_WriteRegU16(block, reg, value) mHSS_WriteRegEx(uint16_t, block, reg, value)
#define mHSS_WriteRegU32(block, reg, value) mHSS_WriteRegEx(uint32_t, block, reg, value)

#define mHSS_WriteRegEx(type, block, reg, value) { *(type*)(block##_BASE_ADDR + block##_##reg##_OFFSET) = value; }
#define mHSS_WriteRegU8(block, reg, value)  mHSS_WriteRegEx(uint8_t, block, reg, value)
#define mHSS_WriteRegU16(block, reg, value) mHSS_WriteRegEx(uint16_t, block, reg, value)
#define mHSS_WriteRegU32(block, reg, value) mHSS_WriteRegEx(uint32_t, block, reg, value)
#define mHSS_WriteRegU64(block, reg, value) mHSS_WriteRegEx(uint64_t, block, reg, value)

#define mHSS_ReadRegEx(type, block, reg) (*(volatile type*)(block##_BASE_ADDR + block##_##reg##_OFFSET))
#define mHSS_ReadRegU8(block, reg)  mHSS_ReadRegEx(uint8_t, block, reg)
#define mHSS_ReadRegU16(block, reg) mHSS_ReadRegEx(uint16_t, block, reg)
#define mHSS_ReadRegU32(block, reg) mHSS_ReadRegEx(uint32_t, block, reg)
#define mHSS_ReadRegU64(block, reg) mHSS_ReadRegEx(uint64_t, block, reg)

#define mHSS_ReadModWriteRegEx(type, block, reg, mask, value)	mHSS_WriteRegEx(type,     block, reg, (mHSS_ReadRegEx(type, block, reg) & mask) | (value & mask))
#define mHSS_ReadModWriteRegU8(block, reg, mask, value)	   	mHSS_WriteRegEx(uint8_t,  block, reg, (mHSS_ReadRegEx(uint8_t,  block, reg) & mask) | (value & mask))
#define mHSS_ReadModWriteRegU16(block, reg, mask, value)	mHSS_WriteRegEx(uint16_t, block, reg, (mHSS_ReadRegEx(uint16_t, block, reg) & mask) | (value & mask))
#define mHSS_ReadModWriteRegU32(block, reg, mask, value)	mHSS_WriteRegEx(uint32_t, block, reg, (mHSS_ReadRegEx(uint32_t, block, reg) & mask) | (value & mask))
#define mHSS_ReadModWriteRegU64(block, reg, mask, value)	mHSS_WriteRegEx(uint64_t, block, reg, (mHSS_ReadRegEx(uint64_t, block, reg) & mask) | (value & mask))

#ifdef __cplusplus
}
#endif

#endif
