/*
 * This file is part of the coreboot project.
 *
 * Copyright 2013 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdint.h>
#include <timer.h>
#define CONFIG_SYS_CLK_FREQ		24000000
static const uint32_t clocks_per_usec = CONFIG_SYS_CLK_FREQ/1000000;
typedef volatile struct tagTIMER_STRUCT
{
	u32 TIMER_LOAD_COUNT0;
	u32 TIMER_LOAD_COUNT1;
	u32 TIMER_CURR_VALUE0;
	u32 TIMER_CURR_VALUE1;
	u32 TIMER_CTRL_REG;
	u32 TIMER_INT_STATUS;
}TIMER_REG,*pTIMER_REG;

#define TIMER_LOAD_VAL	0xffffffff

static uint64_t timer_raw_value(void)
{
	static int enabled = 0;
	uint64_t value0;
	uint64_t value1;
	pTIMER_REG g_Time7Reg = ((pTIMER_REG)0xFF810020);
	if (!enabled) {
		g_Time7Reg->TIMER_LOAD_COUNT0 = TIMER_LOAD_VAL;
		g_Time7Reg->TIMER_LOAD_COUNT1 = TIMER_LOAD_VAL;
		g_Time7Reg->TIMER_CTRL_REG = 0x01;
		enabled = 1;
	}
	value0 =(uint64_t)g_Time7Reg->TIMER_CURR_VALUE0;
	value1 = (uint64_t)g_Time7Reg->TIMER_CURR_VALUE1;
	value0 = value0 | value1<<32;
	return value0;
}
void timer_monotonic_get(struct mono_time *mt)
{
	mono_time_set_usecs(mt, timer_raw_value() /clocks_per_usec);
}
