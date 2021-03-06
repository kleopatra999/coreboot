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

#include <console/console.h>
#include <device/device.h>
#include <soc/rockchip/rk3288/i2c.h>

#define GPIO_BEGUG 0
#define GPIO_WP			GPIO_BEGUG
#define GPIO_DEV  		GPIO_BEGUG
#define GPIO_PWR  		GPIO_BEGUG
#define GPIO_RECOVERY 	GPIO_BEGUG
#define GPIO_LIB			GPIO_BEGUG
static void setup_gpio(void)
{
	gpio_direction_input(GPIO_WP);
	gpio_direction_input(GPIO_DEV);
	gpio_direction_input(GPIO_PWR);
	gpio_direction_input(GPIO_RECOVERY);
	gpio_direction_input(GPIO_LIB);
}

static void mainboard_init(device_t dev)
{
	int i;
	for (i = 0; i < 6; i++)
		i2c_init(i);
	setup_gpio();
}

static void mainboard_enable(device_t dev)
{
	printk(BIOS_INFO, "Enable qemu/armv7 device...\n");
	dev->ops->init = &mainboard_init;
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};

