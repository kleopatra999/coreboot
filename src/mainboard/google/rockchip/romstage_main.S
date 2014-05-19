/*
 * Early initialization code for ARM architecture.
 *
 * This file is based off of the OMAP3530/ARM Cortex start.S file from Das
 * U-Boot, which itself got the file from armboot.
 *
 * Copyright (c) 2004	Texas Instruments <r-woodruff2@ti.com>
 * Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 * Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 * Copyright (c) 2002	Gary Jennejohn <garyj@denx.de>
 * Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
 * Copyright (c) 2003	Kshitij <kshitij@ti.com>
 * Copyright (c) 2006-2008 Syed Mohammed Khasim <x0khasim@ti.com>
 * Copyright (c) 2013   The Chromium OS Authors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <arch/asm.h>


.thumb
ENTRY(main)
	ldr	r0, .Stack
	ldr	r1, .Stack_size
	sub	r0, r0, r1
	ldr	r1, .Stack
	ldr	r2, =0xdeadbeef
init_stack_loop:
	str	r2, [r0]
	add	r0, #4
	cmp	r0, r1
	bne	init_stack_loop

/* Set stackpointer in internal RAM to call bootblock main() */
call_bootblock:
	ldr	sp, .Stack /* Set up stack pointer */
	ldr	r0,=0x00000000
	 /*
	  * The current design of cpu_info places the struct at the top of the
	  * stack. Free enough space to accomodate for that, but make sure it's
	  * 8-byte aligned for ABI compliance.
	  */
	sub	sp, sp, #16
	bl	romstage_init
ENDPROC(main)

/* we do it this way because it's a 32-bit constant and
 * in some cases too far away to be loaded as just an offset
 * from IP
 */
.align 2
.Stack:
	.word CONFIG_ROMSTAGE_STACK_TOP
.align 2
/* create this size the same way we do in coreboot_ram.ld: top-bottom */
.Stack_size:
	.word CONFIG_ROMSTAGE_STACK_TOP - CONFIG_ROMSTAGE_STACK_BOTTOM
