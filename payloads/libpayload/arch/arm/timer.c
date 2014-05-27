
/*
 * Copyright 2013 Google Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <arch/io.h>
#include <libpayload.h>
#include <stdint.h>

typedef volatile struct tagTIMER_STRUCT
{
	u32 TIMER_LOAD_COUNT0;
	u32 TIMER_LOAD_COUNT1;
	u32 TIMER_CURR_VALUE0;
	u32 TIMER_CURR_VALUE1;
	u32 TIMER_CTRL_REG;
	u32 TIMER_INT_STATUS;
}TIMER_REG,*pTIMER_REG;
uint64_t timer_hz(void)
{
	return 24000000;
}

uint64_t timer_raw_value(void)
{
	uint64_t value0;
	uint64_t value1;
	pTIMER_REG g_Time7Reg = ((pTIMER_REG)0xFF810020);
	value0 =(uint64_t)g_Time7Reg->TIMER_CURR_VALUE0;
	value1 = (uint64_t)g_Time7Reg->TIMER_CURR_VALUE1;
	value0 = value0 | value1<<32;
	return value0;
}


u32 cpu_khz;

/**
 * Calculate the speed of the processor for use in delays.
 *
 * @return The CPU speed in kHz.
 */
unsigned int get_cpu_speed(void)
{
	/* FIXME */
	cpu_khz = 24000;

	return cpu_khz;
}







