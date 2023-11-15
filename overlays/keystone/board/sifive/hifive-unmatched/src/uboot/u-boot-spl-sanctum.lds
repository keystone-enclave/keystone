/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Based on arch/riscv/cpu/u-boot.lds, which is
 * Copyright (C) 2017 Andes Technology Corporation
 * Rick Chen, Andes Technology Corporation <rick@andestech.com>
 *
 * and arch/mips/cpu/u-boot-spl.lds.
 */
MEMORY { .spl_mem : ORIGIN = IMAGE_TEXT_BASE, LENGTH = IMAGE_MAX_SIZE }
MEMORY { .bss_mem : ORIGIN = CONFIG_SPL_BSS_START_ADDR, \
		    LENGTH = CONFIG_SPL_BSS_MAX_SIZE }

OUTPUT_ARCH("riscv")
ENTRY(_start)

SECTIONS
{
	. = ALIGN(4);
	.text : {
		arch/riscv/cpu/start.o	(.text)
		*(.text*)
	} > .spl_mem

	. = ALIGN(4);
	.rodata : {
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	} > .spl_mem

	. = ALIGN(4);
	.data : {
		*(.data*)
	} > .spl_mem
	. = ALIGN(4);

	__u_boot_list : {
		KEEP(*(SORT(__u_boot_list*)));
	} > .spl_mem

	. = ALIGN(4);

	.binman_sym_table : {
		__binman_sym_start = .;
		KEEP(*(SORT(.binman_sym*)));
		__binman_sym_end = .;
	} > .spl_mem

	. = ALIGN(8);

	_end = .;
	_image_binary_end = .;

	.bss : {
		__bss_start = .;
		*(.bss*)
		. = ALIGN(8);
		__bss_end = .;
	} > .bss_mem

	. = 0x801ff000; /* the last page before the payload */

	/* ## manufacturer_keys : */

	/* 32 Bytes : manufacturer public key */
	PROVIDE( sanctum_m_public_key = . );
	. += 0x20;

	/* 32 Bytes : device public key */
	PROVIDE( sanctum_dev_public_key = . );
	. += 0x20;

	/* 64 Bytes : device secret key */
	PROVIDE( sanctum_dev_secret_key = . );
	. += 0x40;

	/* ## security_monitor_keys : */

	/* 64 Bytes : security monitor hash */
	PROVIDE( sanctum_sm_hash = . );
	. += 0x40;

	/* 32 Bytes : security monitor public key */
	PROVIDE( sanctum_sm_public_key = . );
	. += 0x20;

	/* 64 Bytes : security monitor secret key */
	PROVIDE( sanctum_sm_secret_key = . );
	. += 0x40;

	/* 64 Bytes : security monitor's signature by device */
	PROVIDE( sanctum_sm_signature = . );
	. += 0x40;
}
