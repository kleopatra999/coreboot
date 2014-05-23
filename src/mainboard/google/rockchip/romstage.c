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
#include <soc/rockchip/rk3288/timer.h>
#include <soc/rockchip/rk3288/dramMain.h>
#include "romstage.h"

void romstage_init(void)
{
	void *entry;
	init_timer();
	rkclk_set_pll();
	console_init();
	dram_main();
	udelay(100);
	cbmem_initialize_empty();
	entry = cbfs_load_stage(CBFS_DEFAULT_MEDIA, "fallback/coreboot_ram");

	stage_exit(entry);
}
