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

#include <device/device.h>
#include <boot/coreboot_tables.h>
#include <soc/nvidia/tegra124/gpio.h>

static void setup_pinmux(void)
{
	// Write protect.
	gpio_input_pullup(GPIO(R1));
	// Recovery mode.
	gpio_input_pullup(GPIO(Q7));
	// Lid switch.
	gpio_input_pullup(GPIO(R4));
	// Power switch.
	gpio_input_pullup(GPIO(Q0));
	// Developer mode.
	gpio_input_pullup(GPIO(Q6));
	// EC in RW.
	gpio_input_pullup(GPIO(U4));

	// SPI1 MOSI
	pinmux_set_config(PINMUX_ULPI_CLK_INDEX, PINMUX_ULPI_CLK_FUNC_SPI1 |
						 PINMUX_PULL_UP |
						 PINMUX_INPUT_ENABLE);
	// SPI1 MISO
	pinmux_set_config(PINMUX_ULPI_DIR_INDEX, PINMUX_ULPI_DIR_FUNC_SPI1 |
						 PINMUX_PULL_UP |
						 PINMUX_INPUT_ENABLE);
	// SPI1 SCLK
	pinmux_set_config(PINMUX_ULPI_NXT_INDEX, PINMUX_ULPI_NXT_FUNC_SPI1 |
						 PINMUX_PULL_NONE |
						 PINMUX_INPUT_ENABLE);
	// SPI1 CS0
	pinmux_set_config(PINMUX_ULPI_STP_INDEX, PINMUX_ULPI_STP_FUNC_SPI1 |
						 PINMUX_PULL_NONE |
						 PINMUX_INPUT_ENABLE);

	// TODO(hungte) Revice pinmux setup, make nice little SoC functions for
	// every single logical thing instead of dumping a wall of code below.
	uint32_t pin_up = PINMUX_PULL_UP | PINMUX_INPUT_ENABLE,
		 pin_up3 = (PINMUX_PULL_UP | PINMUX_INPUT_ENABLE |
			    PINMUX_TRISTATE),
		 pin_down = PINMUX_PULL_DOWN | PINMUX_INPUT_ENABLE,
		 pin_none = PINMUX_PULL_NONE | PINMUX_INPUT_ENABLE;

	// MMC3
	pinmux_set_config(PINMUX_SDMMC3_CLK_INDEX,
			  PINMUX_SDMMC3_CLK_FUNC_SDMMC3 | pin_none);
	pinmux_set_config(PINMUX_SDMMC3_CMD_INDEX,
			  PINMUX_SDMMC3_CMD_FUNC_SDMMC3 | pin_up);
	pinmux_set_config(PINMUX_SDMMC3_DAT0_INDEX,
			  PINMUX_SDMMC3_DAT0_FUNC_SDMMC3 | pin_up);
	pinmux_set_config(PINMUX_SDMMC3_DAT1_INDEX,
			  PINMUX_SDMMC3_DAT1_FUNC_SDMMC3 | pin_up);
	pinmux_set_config(PINMUX_SDMMC3_DAT2_INDEX,
			  PINMUX_SDMMC3_DAT2_FUNC_SDMMC3 | pin_up);
	pinmux_set_config(PINMUX_SDMMC3_DAT3_INDEX,
			  PINMUX_SDMMC3_DAT3_FUNC_SDMMC3 | pin_up);
	pinmux_set_config(PINMUX_SDMMC3_CLK_LB_IN_INDEX,
			  PINMUX_SDMMC3_CLK_LB_IN_FUNC_SDMMC3 | pin_up3);
	pinmux_set_config(PINMUX_SDMMC3_CLK_LB_OUT_INDEX,
			  PINMUX_SDMMC3_CLK_LB_OUT_FUNC_SDMMC3 | pin_down);

	// MMC3 Card Detect pin.
	gpio_input_pullup(GPIO(V2));
	// Enable MMC power.
	gpio_output(GPIO(R0), 1);

	// MMC4
	pinmux_set_config(PINMUX_SDMMC4_CLK_INDEX,
			  PINMUX_SDMMC4_CLK_FUNC_SDMMC4 | pin_none);
	pinmux_set_config(PINMUX_SDMMC4_CMD_INDEX,
			  PINMUX_SDMMC4_CMD_FUNC_SDMMC4 | pin_up);
	pinmux_set_config(PINMUX_SDMMC4_DAT0_INDEX,
			  PINMUX_SDMMC4_DAT0_FUNC_SDMMC4 | pin_up);
	pinmux_set_config(PINMUX_SDMMC4_DAT1_INDEX,
			  PINMUX_SDMMC4_DAT1_FUNC_SDMMC4 | pin_up);
	pinmux_set_config(PINMUX_SDMMC4_DAT2_INDEX,
			  PINMUX_SDMMC4_DAT2_FUNC_SDMMC4 | pin_up);
	pinmux_set_config(PINMUX_SDMMC4_DAT3_INDEX,
			  PINMUX_SDMMC4_DAT3_FUNC_SDMMC4 | pin_up);
	pinmux_set_config(PINMUX_SDMMC4_DAT4_INDEX,
			  PINMUX_SDMMC4_DAT4_FUNC_SDMMC4 | pin_up);
	pinmux_set_config(PINMUX_SDMMC4_DAT5_INDEX,
			  PINMUX_SDMMC4_DAT5_FUNC_SDMMC4 | pin_up);
	pinmux_set_config(PINMUX_SDMMC4_DAT6_INDEX,
			  PINMUX_SDMMC4_DAT6_FUNC_SDMMC4 | pin_up);
	pinmux_set_config(PINMUX_SDMMC4_DAT7_INDEX,
			  PINMUX_SDMMC4_DAT7_FUNC_SDMMC4 | pin_up);
}

static void mainboard_init(device_t dev)
{
	setup_pinmux();
}

static void mainboard_enable(device_t dev)
{
	dev->ops->init = &mainboard_init;
}

struct chip_operations mainboard_ops = {
	.name	= "nyan",
	.enable_dev = mainboard_enable,
};
