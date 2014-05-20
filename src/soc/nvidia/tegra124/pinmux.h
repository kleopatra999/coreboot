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

#ifndef __SOC_NVIDIA_TEGRA124_PINMUX_H__
#define __SOC_NVIDIA_TEGRA124_PINMUX_H__

#include <soc/nvidia/tegra/pinmux.h>
#include <stdint.h>

#define PINMUX_CONSTANTS(index, name, gpio, func0, func1, func2, func3) \
	PINMUX_##name##_INDEX = index, \
	PINMUX_##name##_FUNC_##func0 = 0, \
	PINMUX_##name##_FUNC_##func1 = 1, \
	PINMUX_##name##_FUNC_##func2 = 2, \
	PINMUX_##name##_FUNC_##func3 = 3, \
	PINMUX_GPIO_##gpio = PINMUX_##name##_INDEX

enum {
	PINMUX_CONSTANTS(0, ULPI_DATA0, O1, SPI3, HSI, UA3, ULPI),
	PINMUX_CONSTANTS(1, ULPI_DATA1, O2, SPI3, HSI, UA3, ULPI),
	PINMUX_CONSTANTS(2, ULPI_DATA2, O3, SPI3, HSI, UA3, ULPI),
	PINMUX_CONSTANTS(3, ULPI_DATA3, O4, SPI3, HSI, UA3, ULPI),
	PINMUX_CONSTANTS(4, ULPI_DATA4, O5, SPI2, HSI, UA3, ULPI),
	PINMUX_CONSTANTS(5, ULPI_DATA5, O6, SPI2, HSI, UA3, ULPI),
	PINMUX_CONSTANTS(6, ULPI_DATA6, O7, SPI2, HSI, UA3, ULPI),
	PINMUX_CONSTANTS(7, ULPI_DATA7, O0, SPI2, HSI, UA3, ULPI),
	PINMUX_CONSTANTS(8, ULPI_CLK, Y0, SPI1, SPI5, UD3, ULPI),
	PINMUX_CONSTANTS(9, ULPI_DIR, Y1, SPI1, SPI5, UD3, ULPI),
	PINMUX_CONSTANTS(10, ULPI_NXT, Y2, SPI1, SPI5, UD3, ULPI),
	PINMUX_CONSTANTS(11, ULPI_STP, Y3, SPI1, SPI5, UD3, ULPI),
	PINMUX_CONSTANTS(12, DAP3_FS, P0, I2S2, SPI5, DCA, DCB),
	PINMUX_CONSTANTS(13, DAP3_DIN, P1, I2S2, SPI5, DCA, DCB),
	PINMUX_CONSTANTS(14, DAP3_DOUT, P2, I2S2, SPI5, DCA, RES3),
	PINMUX_CONSTANTS(15, DAP3_SCLK, P3, I2S2, SPI5, RES2, DCB),
	PINMUX_CONSTANTS(16, GPIO_PV0, V0, RES0, RES1, RES2, RES3),
	PINMUX_CONSTANTS(17, GPIO_PV1, V1, RES0, RES1, RES2, RES3),
	PINMUX_CONSTANTS(18, SDMMC1_CLK, Z0, SDMMC1, CLK12M, RES2, RES3),
	PINMUX_CONSTANTS(19, SDMMC1_CMD, Z1, SDMMC1, SPDIF, SPI4, UA3),
	PINMUX_CONSTANTS(20, SDMMC1_DAT3, Y4, SDMMC1, SPDIF, SPI4, UA3),
	PINMUX_CONSTANTS(21, SDMMC1_DAT2, Y5, SDMMC1, PWM0, SPI4, UA3),
	PINMUX_CONSTANTS(22, SDMMC1_DAT1, Y6, SDMMC1, PWM1, SPI4, UA3),
	PINMUX_CONSTANTS(23, SDMMC1_DAT0, Y7, SDMMC1, RES1, SPI4, UA3),
	PINMUX_CONSTANTS(26, CLK2_OUT, W5, EXTPERIPH2, RES1, RES2, RES3),
	PINMUX_CONSTANTS(27, CLK2_REQ, CC5, DAP, RES1, RES2, RES3),
	PINMUX_CONSTANTS(68, HDMI_INT, N7, RES0, RES1, RES2, RES3),
	PINMUX_CONSTANTS(69, DDC_SCL, V4, I2C4, RES1, RES2, RES3),
	PINMUX_CONSTANTS(70, DDC_SDA, V5, I2C4, RES1, RES2, RES3),
	PINMUX_CONSTANTS(89, UART2_RXD, C3, IR3, SPDIF, UA3, SPI4),
	PINMUX_CONSTANTS(90, UART2_TXD, C2, IR3, SPDIF, UA3, SPI4),
	PINMUX_CONSTANTS(91, UART2_RTS_N, J6, UA3, UB3, NOR, SPI4),
	PINMUX_CONSTANTS(92, UART2_CTS_N, J5, UA3, UB3, NOR, SPI4),
	PINMUX_CONSTANTS(93, UART3_TXD, W6, UC3, RES1, NOR, SPI4),
	PINMUX_CONSTANTS(94, UART3_RXD, W7, UC3, RES1, NOR, SPI4),
	PINMUX_CONSTANTS(95, UART3_CTS_N, A1, UC3, SDMMC1, DTV, NOR),
	PINMUX_CONSTANTS(96, UART3_RTS_N, C0, UC3, PWM0, DTV, NOR),
	PINMUX_CONSTANTS(97, GPIO_PU0, U0, OWR, UA3, NOR, RES3),
	PINMUX_CONSTANTS(98, GPIO_PU1, U1, RES0, UA3, NOR, RES3),
	PINMUX_CONSTANTS(99, GPIO_PU2, U2, RES0, UA3, NOR, RES3),
	PINMUX_CONSTANTS(100, GPIO_PU3, U3, PWM0, UA3, NOR, DCB),
	PINMUX_CONSTANTS(101, GPIO_PU4, U4, PWM1, UA3, NOR, DCB),
	PINMUX_CONSTANTS(102, GPIO_PU5, U5, PWM2, UA3, NOR, DCB),
	PINMUX_CONSTANTS(103, GPIO_PU6, U6, PWM3, UA3, RES2, NOR),
	PINMUX_CONSTANTS(104, GEN1_I2C_SDA, C5, I2C1, RES1, RES2, RES3),
	PINMUX_CONSTANTS(105, GEN1_I2C_SCL, C4, I2C1, RES1, RES2, RES3),
	PINMUX_CONSTANTS(106, DAP4_FS, P4, I2S3, NOR, DTV, RES3),
	PINMUX_CONSTANTS(107, DAP4_DIN, P5, I2S3, NOR, RES2, RES3),
	PINMUX_CONSTANTS(108, DAP4_DOUT, P6, I2S3, NOR, DTV, RES3),
	PINMUX_CONSTANTS(109, DAP4_SCLK, P7, I2S3, NOR, RES2, RES3),
	PINMUX_CONSTANTS(110, CLK3_OUT, EE0, EXTPERIPH3, RES1, RES2, RES3),
	PINMUX_CONSTANTS(111, CLK3_REQ, EE1, DEV3, RES1, RES2, RES3),
	PINMUX_CONSTANTS(112, GPIO_PC7, C7, RES0, RES1, NOR_WP_N, NOR_INT1),
	PINMUX_CONSTANTS(113, GPIO_PI5, I5, SDMMC2, RES1, NOR, RES3),
	PINMUX_CONSTANTS(114, GPIO_PI7, I7, RES0, TRACE, NOR, DTV),
	PINMUX_CONSTANTS(115, GPIO_PK0, K0, RES0, SDMMC3, NOR, SOC_THERM),
	PINMUX_CONSTANTS(116, GPIO_PK1, K1, SDMMC2, TRACE, NOR, RES3),
	PINMUX_CONSTANTS(117, GPIO_PJ0, J0, RES0, RES1, NOR, USB),
	PINMUX_CONSTANTS(118, GPIO_PJ2, J2, RES0, RES1, NOR, SOC_THERM),
	PINMUX_CONSTANTS(119, GPIO_PK3, K3, SDMMC2, TRACE, NOR, CCLA),
	PINMUX_CONSTANTS(120, GPIO_PK4, K4, SDMMC2, RES1, NOR_AD22, NOR_INT1),
	PINMUX_CONSTANTS(121, GPIO_PK2, K2, RES0, RES1, NOR, RES3),
	PINMUX_CONSTANTS(122, GPIO_PI3, I3, RES0, RES1, NOR, SPI4),
	PINMUX_CONSTANTS(123, GPIO_PI6, I6, RES0, RES1, NOR, SDMMC2),
	PINMUX_CONSTANTS(124, GPIO_PG0, G0, RES0, RES1, NOR, RES3),
	PINMUX_CONSTANTS(125, GPIO_PG1, G1, RES0, RES1, NOR, RES3),
	PINMUX_CONSTANTS(126, GPIO_PG2, G2, RES0, TRACE, NOR, RES3),
	PINMUX_CONSTANTS(127, GPIO_PG3, G3, RES0, TRACE, NOR, RES3),
	PINMUX_CONSTANTS(128, GPIO_PG4, G4, RES0, TMDS, NOR, SPI4),
	PINMUX_CONSTANTS(129, GPIO_PG5, G5, RES0, RES1, NOR, SPI4),
	PINMUX_CONSTANTS(130, GPIO_PG6, G6, RES0, RES1, NOR, SPI4),
	PINMUX_CONSTANTS(131, GPIO_PG7, G7, RES0, RES1, NOR, SPI4),
	PINMUX_CONSTANTS(132, GPIO_PH0, H0, PWM0, TRACE, NOR, DTV),
	PINMUX_CONSTANTS(133, GPIO_PH1, H1, PWM1, TMDS, NOR, DCA),
	PINMUX_CONSTANTS(134, GPIO_PH2, H2, PWM2, TDMS, NOR, CLDVFS),
	PINMUX_CONSTANTS(135, GPIO_PH3, H3, PWM3, SPI4, NOR, CLDVFS),
	PINMUX_CONSTANTS(136, GPIO_PH4, H4, SDMMC2, RES1, NOR, RES3),
	PINMUX_CONSTANTS(137, GPIO_PH5, H5, SDMMC2, RES1, NOR, RES3),
	PINMUX_CONSTANTS(138, GPIO_PH6, H6, SDMMC2, TRACE, NOR, DTV),
	PINMUX_CONSTANTS(139, GPIO_PH7, H7, SDMMC2, TRACE, NOR, DTV),
	PINMUX_CONSTANTS(140, GPIO_PJ7, J7, UD3, RES1, NOR_AD16, NOR_INT2),
	PINMUX_CONSTANTS(141, GPIO_PB0, B0, UD3, RES1, NOR, RES3),
	PINMUX_CONSTANTS(142, GPIO_PB1, B1, UD3, RES1, NOR, RES3),
	PINMUX_CONSTANTS(143, GPIO_PK7, K7, UD3, RES1, NOR, RES3),
	PINMUX_CONSTANTS(144, GPIO_PI0, I0, RES0, RES1, NOR, RES3),
	PINMUX_CONSTANTS(145, GPIO_PI1, I1, RES0, RES1, NOR, RES3),
	PINMUX_CONSTANTS(146, GPIO_PI2, I2, SDMMC2, TRACE, NOR, RES3),
	PINMUX_CONSTANTS(147, GPIO_PI4, I4, SPI4, TRACE, NOR, DCA),
	PINMUX_CONSTANTS(148, GEN2_I2C_SCL, T5, I2C2, RES1, NOR, RES3),
	PINMUX_CONSTANTS(149, GEN2_I2C_SDA, T6, I2C2, RES1, NOR, RES3),
	PINMUX_CONSTANTS(150, SDMMC4_CLK, CC4, SDMMC4, RES1, NOR, RES3),
	PINMUX_CONSTANTS(151, SDMMC4_CMD, T7, SDMMC4, RES1, NOR, RES3),
	PINMUX_CONSTANTS(152, SDMMC4_DAT0, AA0, SDMMC4, SPI3, NOR, RES3),
	PINMUX_CONSTANTS(153, SDMMC4_DAT1, AA1, SDMMC4, SPI3, NOR, RES3),
	PINMUX_CONSTANTS(154, SDMMC4_DAT2, AA2, SDMMC4, SPI3, NOR, RES3),
	PINMUX_CONSTANTS(155, SDMMC4_DAT3, AA3, SDMMC4, SPI3, NOR, RES3),
	PINMUX_CONSTANTS(156, SDMMC4_DAT4, AA4, SDMMC4, SPI3, NOR, RES3),
	PINMUX_CONSTANTS(157, SDMMC4_DAT5, AA5, SDMMC4, SPI3, RES2, RES3),
	PINMUX_CONSTANTS(158, SDMMC4_DAT6, AA6, SDMMC4, SPI3, NOR, RES3),
	PINMUX_CONSTANTS(159, SDMMC4_DAT7, AA7, SDMMC4, RES1, NOR, RES3),
	PINMUX_CONSTANTS(161, CAM_MCLK, CC0, VIMCLK_PRI, VIMCLK_ALT1,
			 VIMCLK_ALT3, SDMMC2),
	PINMUX_CONSTANTS(162, GPIO_PCC1, CC1, I2S4, RES1, RES2, SDMMC2),
	PINMUX_CONSTANTS(163, GPIO_PBB0, BB0, VGP6, VIMCLK2_PRI, SDMMC2,
			 VIMCLK2_ALT3),
	PINMUX_CONSTANTS(164, CAM_I2C_SCL, BB1, VGP1, I2C3, RES2, SDMMC2),
	PINMUX_CONSTANTS(165, CAM_I2C_SDA, BB2, VGP2, I2C3, RES2, SDMMC2),
	PINMUX_CONSTANTS(166, GPIO_PBB3, BB3, VGP3, DCA, DCB, SDMMC2),
	PINMUX_CONSTANTS(167, GPIO_PBB4, BB4, VGP4, DCA, DCB, SDMMC2),
	PINMUX_CONSTANTS(168, GPIO_PBB5, BB5, VGP5, DCA, RES2, SDMMC2),
	PINMUX_CONSTANTS(169, GPIO_PBB6, BB6, I2S4, RES1, DCB, SDMMC2),
	PINMUX_CONSTANTS(170, GPIO_PBB7, BB7, I2S4, RES1, RES2, SDMMC2),
	PINMUX_CONSTANTS(171, GPIO_PCC2, CC2, I2S4, RES1, SDMMC3, SDMMC2),
	PINMUX_CONSTANTS(172, JTAG_RTCK, NONE172, RTCK, RES1, RES2, RES3),
	PINMUX_CONSTANTS(173, PWR_I2C_SCL, Z6, I2CPMU, RES1, RES2, RES3),
	PINMUX_CONSTANTS(174, PWR_I2C_SDA, Z7, I2CPMU, RES1, RES2, RES3),
	PINMUX_CONSTANTS(175, KB_ROW0, R0, RES0, RES1, RES2, RES3),
	PINMUX_CONSTANTS(176, KB_ROW1, R1, RES0, RES1, RES2, RES3),
	PINMUX_CONSTANTS(177, KB_ROW2, R2, RES0, RES1, RES2, RES3),
	PINMUX_CONSTANTS(178, KB_ROW3, R3, RES0, DCA, SYS_CLK, DCB),
	PINMUX_CONSTANTS(179, KB_ROW4, R4, RES0, DCA, RES2, DCB),
	PINMUX_CONSTANTS(180, KB_ROW5, R5, RES0, DCA, RES2, DCB),
	PINMUX_CONSTANTS(181, KB_ROW6, R6, RES0, DCA_LSC0, DCA_LSPII, DCB),
	PINMUX_CONSTANTS(182, KB_ROW7, R7, RES0, RES1, CLDVFS, UA3),
	PINMUX_CONSTANTS(183, KB_ROW8, S0, RES0, RES1, CLDVFS, UA3),
	PINMUX_CONSTANTS(184, KB_ROW9, S1, RES0, RES1, RES2, UA3),
	PINMUX_CONSTANTS(185, KB_ROW10, S2, RES0, RES1, RES2, UA3),
	PINMUX_CONSTANTS(186, KB_ROW11, S3, RES0, RES1, RES2, IR3),
	PINMUX_CONSTANTS(187, KB_ROW12, S4, RES0, RES1, RES2, IR3),
	PINMUX_CONSTANTS(188, KB_ROW13, S5, RES0, RES1, SPI2, RES3),
	PINMUX_CONSTANTS(189, KB_ROW14, S6, RES0, RES1, SPI2, RES3),
	PINMUX_CONSTANTS(190, KB_ROW15, S7, RES0, SOC_THERM, RES2, RES3),
	PINMUX_CONSTANTS(191, KB_COL0, Q0, RES0, RES1, SPI2, RES3),
	PINMUX_CONSTANTS(192, KB_COL1, Q1, RES0, RES1, SPI2, RES3),
	PINMUX_CONSTANTS(193, KB_COL2, Q2, RES0, RES1, SPI2, RES3),
	PINMUX_CONSTANTS(194, KB_COL3, Q3, RES0, DCA, PWM2, UA3),
	PINMUX_CONSTANTS(195, KB_COL4, Q4, RES0, OWR, SDMMC3, UA3),
	PINMUX_CONSTANTS(196, KB_COL5, Q5, RES0, RES1, SDMMC3, RES3),
	PINMUX_CONSTANTS(197, KB_COL6, Q6, RES0, RES1, SPI2, UD3),
	PINMUX_CONSTANTS(198, KB_COL7, Q7, RES0, RES1, SPI2, UD3),
	PINMUX_CONSTANTS(199, CLK_32K_OUT, A0, BLINK, SOC_THERM, RES2, RES3),
	PINMUX_CONSTANTS(201, CORE_PWR_REQ, NONE201, PWRON, RES1, RES2, RES3),
	PINMUX_CONSTANTS(202, CPU_PWR_REQ, NONE202, CPU, RES1, RES2, RES3),
	PINMUX_CONSTANTS(203, PWR_INT_N, NONE203, PMICINTR, RES1, RES2, RES3),
	PINMUX_CONSTANTS(204, CLK_32K_IN, NONE204, CLK_32K_IN, RES1, RES2,
			 RES3),
	PINMUX_CONSTANTS(205, OWR, NONE205, OWR, RES1, RES2, RES3),
	PINMUX_CONSTANTS(206, DAP1_FS, N0, I2S0, DAP1, NOR, RES3),
	PINMUX_CONSTANTS(207, DAP1_DIN, N1, I2S0, DAP1, NOR, RES3),
	PINMUX_CONSTANTS(208, DAP1_DOUT, N2, I2S0, DAP1, NOR, SATA),
	PINMUX_CONSTANTS(209, DAP1_SCLK, N3, I2S0, DAP1, NOR, RES3),
	PINMUX_CONSTANTS(210, DAP_MCLK1_REQ, EE2, DAP, DAP1, SATA, RES3),
	PINMUX_CONSTANTS(211, DAP_MCLK1, W4, EXTPERIPH1, DAP2, RES2, RES3),
	PINMUX_CONSTANTS(212, SPDIF_IN, K6, SPDIF, RES1, RES2, I2C3),
	PINMUX_CONSTANTS(213, SPDIF_OUT, K5, SPDIF, RES1, RES2, I2C3),
	PINMUX_CONSTANTS(214, DAP2_FS, A2, I2S1, DAP2, NOR, RES3),
	PINMUX_CONSTANTS(215, DAP2_DIN, A4, I2S1, DAP2, NOR, RES3),
	PINMUX_CONSTANTS(216, DAP2_DOUT, A5, I2S1, DAP2, NOR, RES3),
	PINMUX_CONSTANTS(217, DAP2_SCLK, A3, I2S1, SAP2, NOR, RES3),
	PINMUX_CONSTANTS(218, DVFS_PWM, X0, SPI6, CLDVFS, NOR, RES3),
	PINMUX_CONSTANTS(219, GPIO_X1_AUD, X1, SPI6, RES1, NOR, RES3),
	PINMUX_CONSTANTS(220, GPIO_X3_AUD, X3, SPI6, SPI1, NOR, RES3),
	PINMUX_CONSTANTS(221, DVFS_CLK, X2, SPI6, CLDVFS_CLK, NOR, RES3),
	PINMUX_CONSTANTS(222, GPIO_X4_AUD, X4, NOR, SPI1, SPI2, DAP2),
	PINMUX_CONSTANTS(223, GPIO_X5_AUD, X5, NOR, SPI1, SPI2, RES3),
	PINMUX_CONSTANTS(224, GPIO_X6_AUD, X6, SPI6, SPI1, SPI2, NOR),
	PINMUX_CONSTANTS(225, GPIO_X7_AUD, X7, RES0, SPI1, SPI2, RES3),
	PINMUX_CONSTANTS(228, SDMMC3_CLK, A6, SDMMC3, RES1, RES2, SPI3),
	PINMUX_CONSTANTS(229, SDMMC3_CMD, A7, SDMMC3, PWM3, UA3, SPI3),
	PINMUX_CONSTANTS(230, SDMMC3_DAT0, B7, SDMMC3, RES1, RES2, SPI3),
	PINMUX_CONSTANTS(231, SDMMC3_DAT1, B6, SDMMC3, PWM2, UA3, SPI3),
	PINMUX_CONSTANTS(232, SDMMC3_DAT2, B5, SDMMC3, PWM1, DCA, SPI3),
	PINMUX_CONSTANTS(233, SDMMC3_DAT3, B4, SDMMC3, PWM0, DCB, SPI3),
	PINMUX_CONSTANTS(239, PEX_L0_RST_N, DD1, PE0, RES1, RES2, RES3),
	PINMUX_CONSTANTS(240, PEX_L0_CLKREQ_N, DD2, PE0, RES1, RES2, RES3),
	PINMUX_CONSTANTS(241, PEX_WAKE_N, DD3, PE, RES1, RES2, RES3),
	PINMUX_CONSTANTS(243, PEX_L1_RST_N, DD5, PE1, RES1, RES2, RES3),
	PINMUX_CONSTANTS(244, PEX_L1_CLKREQ_N, DD6, PE1, RES1, RES2, RES3),
	PINMUX_CONSTANTS(248, HDMI_CEC, EE3, CEC, RES1, RES2, RES3),
	PINMUX_CONSTANTS(249, SDMMC1_WP_N, V3, SDMMC1, CLK12M, SPI4, UA3),
	PINMUX_CONSTANTS(250, SDMMC3_CD_N, V2, SDMMC3, OWR, RES2, RES3),
	PINMUX_CONSTANTS(251, GPIO_W2_AUD, W2, SPI6, RES1, SPI2, I2C1),
	PINMUX_CONSTANTS(252, GPIO_W3_AUD, W3, SPI6, SPI1, SPI2, I2C1),
	PINMUX_CONSTANTS(253, USB_VBUS_EN0, N4, USB, RES1, RES2, RES3),
	PINMUX_CONSTANTS(254, USB_VBUS_EN1, N5, USB, RES1, RES2, RES3),
	PINMUX_CONSTANTS(255, SDMMC3_CLK_LB_IN, EE5, SDMMC3, RES1, RES2, RES3),
	PINMUX_CONSTANTS(256, SDMMC3_CLK_LB_OUT, EE4, SDMMC3, RES1, RES2, RES3),
	PINMUX_CONSTANTS(258, RESET_OUT_N, NONE258, RES0, RES1, RES2, RESET),
	PINMUX_CONSTANTS(259, KB_ROW16, T0, RES0, RES1, RES2, UC3),
	PINMUX_CONSTANTS(260, KB_ROW17, T1, RES0, RES1, RES2, UC3),
	PINMUX_CONSTANTS(261, USB_VBUS_EN2, FF1, USB, RES1, RES2, RES3),
	PINMUX_CONSTANTS(262, GPIO_PFF2, FF2, SATA, RES1, RES2, RES3),
	PINMUX_CONSTANTS(268, DP_HPD, FF0, DP, RES1, RES2, RES3),

	// Where do these go?
	/*
	PINMUX_JTAG_TRST_N_JTAG_TRST_N = 0,

	PINMUX_JTAG_TDO_JTAG_TDO = 0,

	PINMUX_JTAG_TMS_JTAG_TMS = 0,

	PINMUX_JTAG_TCK_JTAG_TCK = 0,
	// What do functions 1 and 2 do?

	PINMUX_JTAG_TDI_JTAG_TDI = 0,
	PINMUX_JTAG_TDI_PWR_BREAK = 1,

	PINMUX_TEST_MODE_EN_TEST_MODE_EN = 0,
	PINMUX_TEST_MODE_EN_VIMCLK_PRI = 1,
	PINMUX_TEST_MODE_EN_VIMCLK_ALT1 = 2,
	PINMUX_TEST_MODE_EN_VIMCLK_ALT3 = 3,

	PINMUX_DP_AUX_CH0_P_I2C_CLK = 0,
	PINMUX_DP_AUX_CH0_P_SDMMC3_DAT3 = 1,
	PINMUX_DP_AUX_CH0_P_PM3_PWM0 = 2,
	PINMUX_DP_AUX_CH0_P_DCB_LPM0 = 3,

	PINMUX_DP_AUX_CH0_N_I2C6_DAT = 0,
	PINMUX_DP_AUX_CH0_N_SDMMC3_DAT2 = 1,
	PINMUX_DP_AUX_CH0_N_PM3_PWM1 = 2,
	PINMUX_DP_AUX_CH0_N_DCA_LPM0 = 3,
	*/
};

#endif	/* __SOC_NVIDIA_TEGRA124_PINMUX_H__ */
