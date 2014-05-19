/*
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
#include <console/console.h>
#include <arch/cache.h>
#include "grf.h"
#include "memap.h"
#include "pinmux.h"

void bootblock_mainboard_init(void);
void bootblock_cpu_init(void);

void bootblock_cpu_init(void)
{
	
}

void bootblock_mainboard_init(void)
{
	rk_iomux_config(RK_UART_DBG_IOMUX);
	rk_iomux_config(RK_SPI2_CS0_IOMUX);
}