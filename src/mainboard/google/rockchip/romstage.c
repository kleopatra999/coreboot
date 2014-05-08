/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2013 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <types.h>
#include <armv7.h>
#include <cbfs.h>
#include <console/console.h>
#include <arch/stages.h>
#include <cbmem.h>
#include <timestamp.h>
#include <arch/cache.h>
#include <arch/exception.h>
#include <soc/rockchip/rk3288/clock.h>



void main(void)
{
	void *entry;

	rkclk_set_pll();
	console_init();
	cbmem_initialize_empty();
	entry = cbfs_load_stage(CBFS_DEFAULT_MEDIA, "fallback/coreboot_ram");

	stage_exit(entry);
}
