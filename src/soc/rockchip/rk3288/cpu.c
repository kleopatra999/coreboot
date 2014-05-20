/*
 * This file is part of the coreboot project.
 *
 * Copyright 2013 Google Inc.
 * Copyright (C) 2012 Samsung Electronics
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

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <delay.h>
#include <console/console.h>
#include <device/device.h>
#include <cbmem.h>
#include <arch/cache.h>

#include "cpu.h"

static void cpu_enable(device_t dev)
{
	ram_resource(dev, 0, RAM_BASE_KB, RAM_SIZE_KB - FB_SIZE_KB);
}
static void cpu_init(device_t dev)
{

}

static void cpu_noop(device_t dev)
{
}

static struct device_operations cpu_ops = {
	.read_resources   = cpu_noop,
	.set_resources    = cpu_noop,
	.enable_resources = cpu_enable,
	.init             = cpu_init,
	.scan_bus         = 0,
};

static void enable_rk3288_dev(device_t dev)
{
	dev->ops = &cpu_ops;
}

struct chip_operations soc_rockchip_rk3288_ops = {
	CHIP_NAME("SOC Rockchip 3288")
	.enable_dev = enable_rk3288_dev,
};

