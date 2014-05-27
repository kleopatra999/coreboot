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
#include <soc/rockchip/rk3288/dramMain.h>
#include "romstage.h"

void romstage_init(void)
{
	void *entry;	
#if CONFIG_COLLECT_TIMESTAMPS
	uint64_t start_romstage_time;
	uint64_t before_dram_time;
	uint64_t after_dram_time;
	uint64_t base_time = timestamp_get();
	start_romstage_time = timestamp_get();
#endif
	rkclk_set_pll();
	console_init();
#if CONFIG_COLLECT_TIMESTAMPS
	before_dram_time = timestamp_get();
#endif
	dram_main();
#if CONFIG_COLLECT_TIMESTAMPS
	after_dram_time = timestamp_get();
#endif
	udelay(100);
	cbmem_initialize_empty();
#if CONFIG_COLLECT_TIMESTAMPS
	timestamp_init(base_time);
	timestamp_add(TS_START_ROMSTAGE, start_romstage_time );
	timestamp_add(TS_BEFORE_INITRAM, before_dram_time );
	timestamp_add(TS_AFTER_INITRAM, after_dram_time );
#endif
	entry = cbfs_load_stage(CBFS_DEFAULT_MEDIA, "fallback/coreboot_ram");
#if CONFIG_COLLECT_TIMESTAMPS
	timestamp_add_now(TS_END_ROMSTAGE);
#endif
	stage_exit(entry);
}
