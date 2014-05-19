/*
 * (C) Copyright 2008-2013 Rockchip Electronics
 * Peter, Software Engineering, <superpeter.cai@gmail.com>.
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
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <assert.h>
#include <stdlib.h>
#include <arch/io.h>
#include <stdint.h>
#include <console/console.h>
#include "clock.h"
#include "reg.h"
#include "memap.h"
#include <delay.h>

#define CONFIG_RKCLK_APLL_FREQ		1008 /* MHZ */
#define CONFIG_RKCLK_GPLL_FREQ		594 /* MHZ */
#define CONFIG_RKCLK_CPLL_FREQ		594 /* MHZ */



/* pll set callback function */
struct pll_clk_set;
typedef void (*pll_callback_f)(struct pll_clk_set *clkset);


/****************************************************************************
Internal sram us delay function
Cpu highest frequency is 1 GHz
1 cycle = 1/1 ns
1 us = 1000 ns = 1000 * 1 cycles = 1000 cycles
*****************************************************************************/
#define LPJ_1000MHZ  100UL
#define nop() __asm__ __volatile__("mov\tr0,r0\t@ nop\n\t");
static void clk_loop_delayus(uint32_t us)
{   
	volatile uint32_t i;

	/* copro seems to need some delay between reading and writing */
	for (i = 0; i < LPJ_1000MHZ * us; i++) {
		nop();
	}
}


/* define clock sourc div */
#define CLK_DIV_1		1
#define CLK_DIV_2		2
#define CLK_DIV_3		3
#define CLK_DIV_4		4
#define CLK_DIV_5		5
#define CLK_DIV_6		6
#define CLK_DIV_7		7
#define CLK_DIV_8		8
#define CLK_DIV_9		9
#define CLK_DIV_10		10
#define CLK_DIV_11		11
#define CLK_DIV_12		12
#define CLK_DIV_13		13
#define CLK_DIV_14		14
#define CLK_DIV_15		15
#define CLK_DIV_16		16
#define CLK_DIV_17		17
#define CLK_DIV_18		18
#define CLK_DIV_19		19
#define CLK_DIV_20		20
#define CLK_DIV_21		21
#define CLK_DIV_22		22
#define CLK_DIV_23		23
#define CLK_DIV_24		24
#define CLK_DIV_25		25
#define CLK_DIV_26		26
#define CLK_DIV_27		27
#define CLK_DIV_28		28
#define CLK_DIV_29		29
#define CLK_DIV_30		30
#define CLK_DIV_31		31
#define CLK_DIV_32		32


enum rk_plls_id {
	APLL_ID = 0,
	DPLL_ID,
	CPLL_ID,
	GPLL_ID,
	NPLL_ID,

	END_PLL_ID
};


/*****cru reg offset*****/

#define CRU_MODE_CON		0x50
#define CRU_CLKSEL_CON		0x60
#define CRU_CLKGATE_CON		0x160
#define CRU_GLB_SRST_FST	0x1B0
#define CRU_GLB_SRST_SND	0x1B4
#define CRU_SOFTRST_CON		0x1B8

#define PLL_CONS(id, i)		((id) * 0x10 + ((i) * 4))

#define CRU_CLKSELS_CON_CNT	(43)
#define CRU_CLKSELS_CON(i)	(CRU_CLKSEL_CON + ((i) * 4))

#define CRU_CLKGATES_CON_CNT	(19)
#define CRU_CLKGATES_CON(i)	(CRU_CLKGATE_CON + ((i) * 4))

#define CRU_SOFTRSTS_CON_CNT	(12)
#define CRU_SOFTRSTS_CON(i)	(CRU_SOFTRST_CON + ((i) * 4))

#define CRU_MISC_CON		(0x1E8)
#define CRU_GLB_CNT_TH		(0x1EC)
#define CRU_GLB_RST_CON		(0x1F0)
#define CRU_GLB_RST_ST		(0x1F8)

#define CRU_SDMMC_CON0		(0x200)
#define CRU_SDMMC_CON1		(0x204)
#define CRU_SDIO0_CON0		(0x208)
#define CRU_SDIO0_CON1		(0x20C)
#define CRU_SDIO1_CON0		(0x210)
#define CRU_SDIO1_CON1		(0x214)
#define CRU_EMMC_CON0		(0x218)
#define CRU_EMMC_CON1		(0x21C)


/********************************************************************/
#define CRU_GET_REG_BITS_VAL(reg, bits_shift, msk)	(((reg) >> (bits_shift))&(msk))
#define CRU_W_MSK(bits_shift, msk)			((msk) << ((bits_shift) + 16))

#define CRU_SET_BITS(val, bits_shift, msk)		(((val)&(msk)) << (bits_shift))
#define CRU_W_MSK_SETBITS(val, bits_shift, msk) 	(CRU_W_MSK(bits_shift, msk)|CRU_SET_BITS(val, bits_shift, msk))


/*******************PLL CON0 BITS***************************/

#define PLL_CLKFACTOR_SET(val, shift, msk)	((((val) - 1) & (msk)) << (shift))
#define PLL_CLKFACTOR_GET(reg, shift, msk)	((((reg) >> (shift)) & (msk)) + 1)

#define PLL_OD_MSK		(0x0F)
#define PLL_OD_SHIFT 		(0x0)

#define PLL_CLKOD(val)		PLL_CLKFACTOR_SET(val, PLL_OD_SHIFT, PLL_OD_MSK)
#define PLL_NO(reg)		PLL_CLKFACTOR_GET(reg, PLL_OD_SHIFT, PLL_OD_MSK)
#define PLL_NO_SHIFT(reg)	PLL_CLKFACTOR_GET(reg, PLL_OD_SHIFT, PLL_OD_MSK)
#define PLL_CLKOD_SET(val)	(PLL_CLKOD(val) | CRU_W_MSK(PLL_OD_SHIFT, PLL_OD_MSK))

#define PLL_NR_MSK		(0x3F)
#define PLL_NR_SHIFT		(8)
#define PLL_CLKR(val)		PLL_CLKFACTOR_SET(val, PLL_NR_SHIFT, PLL_NR_MSK)
#define PLL_NR(reg)		PLL_CLKFACTOR_GET(reg, PLL_NR_SHIFT, PLL_NR_MSK)
#define PLL_CLKR_SET(val)	(PLL_CLKR(val) | CRU_W_MSK(PLL_NR_SHIFT, PLL_NR_MSK))


/*******************PLL CON1 BITS***************************/

#define PLL_NF_MSK		(0x1FFF)
#define PLL_NF_SHIFT		(0)
#define PLL_CLKF(val)		PLL_CLKFACTOR_SET(val, PLL_NF_SHIFT, PLL_NF_MSK)
#define PLL_NF(reg)		PLL_CLKFACTOR_GET(reg, PLL_NF_SHIFT, PLL_NF_MSK)
#define PLL_CLKF_SET(val)	(PLL_CLKF(val) | CRU_W_MSK(PLL_NF_SHIFT, PLL_NF_MSK))


/*******************PLL CON2 BITS***************************/

#define PLL_BWADJ_MSK		(0x0FFF)
#define PLL_BWADJ_SHIFT		(0)
#define PLL_CLK_BWADJ_SET(val)	((val) | CRU_W_MSK(PLL_BWADJ_SHIFT, PLL_BWADJ_MSK))


/*******************PLL CON3 BITS***************************/

#define PLL_BYPASS_MSK		(1 << 0)
#define PLL_BYPASS_W_MSK	(PLL_BYPASS_MSK << 16)
#define PLL_BYPASS		(1 << 0)
#define PLL_NO_BYPASS		(0 << 0)

#define PLL_PWR_DN_MSK		(1 << 1)
#define PLL_PWR_DN_W_MSK	(PLL_PWR_DN_MSK << 16)
#define PLL_PWR_DN		(1 << 1)
#define PLL_PWR_ON		(0 << 1)

#define PLL_FASTEN_MSK		(1 << 2)
#define PLL_FASTEN_W_MSK	(PLL_FASTEN_MSK << 16)
#define PLL_FASTEN		(1 << 2)
#define PLL_NO_FASTEN		(0 << 2)

#define PLL_ENSAT_MSK		(1 << 3)
#define PLL_ENSAT_W_MSK		(PLL_ENSAT_MSK << 16)
#define PLL_ENSAT		(1 << 3)
#define PLL_NO_ENSAT		(0 << 3)

#define PLL_RESET_MSK		(1 << 5)
#define PLL_RESET_W_MSK		(PLL_RESET_MSK << 16)
#define PLL_RESET		(1 << 5)
#define PLL_RESET_RESUME	(0 << 5)


/*******************MODE BITS***************************/

/*note: npll mode bit: bit14-bit15 */
#define PLL_MODE_MSK(id)	((id == NPLL_ID) ? (0x3 << 14) : (0x3 << ((id) * 4)))
#define PLL_MODE_SLOW(id)	((id == NPLL_ID) ? (0x0<<14)|(0x3<<(16+14)) : ((0x0<<((id)*4))|(0x3<<(16+(id)*4))))
#define PLL_MODE_NORM(id)	((id == NPLL_ID) ? (0x1<<14)|(0x3<<(16+14)) : ((0x1<<((id)*4))|(0x3<<(16+(id)*4))))
#define PLL_MODE_DEEP(id)	((id == NPLL_ID) ? (0x2<<14)|(0x3<<(16+14)) : ((0x2<<((id)*4))|(0x3<<(16+(id)*4))))


/*******************CLKSEL0 BITS***************************/
/* core clk pll sel: amr or general */
#define CORE_SEL_PLL_W_MSK	(1 << (15 + 16))
#define CORE_SEL_PLL_MSK	(1 << 15)
#define CORE_SEL_APLL		(0 << 15)
#define CORE_SEL_GPLL		(1 << 15)

/* a12 core clock div: clk_core = clk_src / (div_con + 1) */
#define A12_CORE_CLK_DIV_OFF 	8
#define A12_CORE_CLK_DIV_W_MSK	(0x1F << (8 + 16))
#define A12_CORE_CLK_DIV_MSK	(0x1F << 8)
#define A12_CORE_CLK_DIV(i)	((((i) - 1) & 0x1F) << 8)

/* mp core axi clock div: clk = clk_src / (div_con + 1) */
#define MP_AXI_CLK_DIV_OFF 	4
#define MP_AXI_CLK_DIV_W_MSK	(0xF << (4 + 16))
#define MP_AXI_CLK_DIV_MSK	(0xF << 4)
#define MP_AXI_CLK_DIV(i)	((((i) - 1) & 0xF) << 4)

/* m0 core axi clock div: clk = clk_src / (div_con + 1) */
#define M0_AXI_CLK_DIV_OFF 	0
#define M0_AXI_CLK_DIV_W_MSK	(0xF << (0 + 16))
#define M0_AXI_CLK_DIV_MSK	(0xF << 0)
#define M0_AXI_CLK_DIV(i)	((((i) - 1) & 0xF) << 0)


/*******************CLKSEL1 BITS***************************/

/* pd bus clk pll sel: codec or general */
#define PDBUS_SEL_PLL_W_MSK	(1 << (15 + 16))
#define PDBUS_SEL_PLL_MSK	(1 << 15)
#define PDBUS_SEL_CPLL		(0 << 15)
#define PDBUS_SEL_GPLL		(1 << 15)

/* pd bus pclk div: clk = clk_src / (div_con + 1) */
#define PDBUS_PCLK_DIV_OFF 	12
#define PDBUS_PCLK_DIV_W_MSK	(0x7 << (12 + 16))
#define PDBUS_PCLK_DIV_MSK	(0x7 << 12)
#define PDBUS_PCLK_DIV(i)	((((i) - 1) & 0x7) << 12)

/* pd bus hclk div: aclk_bus: hclk_bus = 1:1 or 2:1 or 4:1 */
#define PDBUS_HCLK_DIV_OFF 	8
#define PDBUS_HCLK_DIV_W_MSK	(0x3 << (8 + 16))
#define PDBUS_HCLK_DIV_MSK	(0x3 << 8)
#define PDBUS_ACLK2HCLK_11	(0 << 8)
#define PDBUS_ACLK2HCLK_21	(1 << 8)
#define PDBUS_ACLK2HCLK_41	(2 << 8)

/* pd bus aclk div: clk = clk_src / (div_con + 1) */
#define PDBUS_ACLK_DIV_OFF 	3
#define PDBUS_ACLK_DIV_W_MSK	(0x1F << (3 + 16))
#define PDBUS_ACLK_DIV_MSK	(0x1F << 3)
#define PDBUS_ACLK_DIV(i)	((((i) - 1) & 0x1F) << 3)

/* pd bus axi div: clk = clk_src / (div_con + 1) */
#define PDBUS_AXI_DIV_OFF 	0
#define PDBUS_AXI_DIV_W_MSK	(0x3 << (0 + 16))
#define PDBUS_AXI_DIV_MSK	(0x3 << 0)
#define PDBUS_AXI_DIV(i)	((((i) - 1) & 0x3) << 0)


/*******************CLKSEL10 BITS***************************/

/* peripheral bus clk pll sel: codec or general */
#define PERI_SEL_PLL_W_MSK	(1 << (15 + 16))
#define PERI_SEL_PLL_MSK	(1 << 15)
#define PERI_SEL_CPLL		(0 << 15)
#define PERI_SEL_GPLL		(1 << 15)

/* peripheral bus pclk div: aclk_bus: pclk_bus = 1:1 or 2:1 or 4:1 or 8:1 */
#define PERI_PCLK_DIV_OFF 	12
#define PERI_PCLK_DIV_W_MSK	(0x7 << (12 + 16))
#define PERI_PCLK_DIV_MSK	(0x7 << 12)
#define PERI_ACLK2PCLK_11	(0 << 12)
#define PERI_ACLK2PCLK_21	(1 << 12)
#define PERI_ACLK2PCLK_41	(2 << 12)
#define PERI_ACLK2PCLK_81	(3 << 12)

/* peripheral bus hclk div: aclk_bus: hclk_bus = 1:1 or 2:1 or 4:1 */
#define PERI_HCLK_DIV_OFF 	8
#define PERI_HCLK_DIV_W_MSK	(0x3 << (8 + 16))
#define PERI_HCLK_DIV_MSK	(0x3 << 8)
#define PERI_ACLK2HCLK_11	(0 << 8)
#define PERI_ACLK2HCLK_21	(1 << 8)
#define PERI_ACLK2HCLK_41	(2 << 8)

/* peripheral bus aclk div: aclk_periph = periph_clk_src / (peri_aclk_div_con + 1) */
#define PERI_ACLK_DIV_OFF	0
#define PERI_ACLK_DIV_W_MSK	(0x1F << (0 + 16))
#define PERI_ACLK_DIV_MSK 	(0x1F << 0)
#define PERI_ACLK_DIV(i)	((((i) - 1) & 0x1F) << 0)


/*******************GATE BITS***************************/

#define CLK_GATE_CLKID(i)	(16 * (i))
#define CLK_GATE_CLKID_CONS(i)	CRU_CLKGATES_CON((i) / 16)

#define CLK_GATE(i)		(1 << ((i)%16))
#define CLK_UN_GATE(i)		(0)

#define CLK_GATE_W_MSK(i)	(1 << (((i) % 16) + 16))



/* Cpu clock source select */
#define CPU_SRC_ARM_PLL			0
#define CPU_SRC_GENERAL_PLL		1

/* Periph clock source select */
#define PERIPH_SRC_GENERAL_PLL		1
#define PERIPH_SRC_CODEC_PLL		0

/* DDR clock source select */
#define DDR_SRC_DDR_PLL			0
#define DDR_SRC_GENERAL_PLL		1

//GRF_SOC_STATUS1
#define DPLL_LOCK  (1<<5)
#define APLL_LOCK  (1<<6)
#define CPLL_LOCK  (1<<7)
#define GPLL_LOCK  (1<<8)
#define NPLL_LOCK  (1<<9)

struct pll_clk_set {
	unsigned long	rate;
	u32	pllcon0;
	u32	pllcon1;
	u32	pllcon2; //bit0 - bit11: nb = bwadj+1 = nf/2
	u32	rst_dly; //us
	u8	a12_core_div;
	u8	aclk_core_mp_div;
	u8	aclk_core_m0_div;
	u8	pclk_dbg_div;
	u8	atclk_core_div;
	u8	l2ram_div;
	u8	aclk_div;
	u8	hclk_div;
	u8	pclk_div;
};

#define cru_readl(offset)	readl((const void *)(CRU_BASE_ADDR + offset))
#define cru_writel(v, offset)	do { writel(v, (int*)(CRU_BASE_ADDR + offset)); } while (0)


#define _APLL_SET_CLKS(khz, nr, nf, no, _a12_div, _mp_div, _m0_div, _l2_div, _atclk_div, _pclk_dbg_div) \
{ \
	.rate			= khz * KHZ, \
	.pllcon0		= PLL_CLKR_SET(nr) | PLL_CLKOD_SET(no), \
	.pllcon1		= PLL_CLKF_SET(nf), \
	.pllcon2		= PLL_CLK_BWADJ_SET(nf >> 1), \
	.a12_core_div		= CLK_DIV_##_a12_div, \
	.aclk_core_mp_div	= CLK_DIV_##_mp_div, \
	.aclk_core_m0_div	= CLK_DIV_##_m0_div, \
	.pclk_dbg_div		= CLK_DIV_##_pclk_dbg_div, \
	.atclk_core_div		= CLK_DIV_##_atclk_div, \
	.l2ram_div		= CLK_DIV_##_l2_div, \
	.rst_dly		= ((nr*500)/24+1), \
}

#define _GPLL_SET_CLKS(khz, nr, nf, no, _axi_div, _ahb_div, _apb_div) \
{ \
	.rate		= khz * KHZ, \
	.pllcon0	= PLL_CLKR_SET(nr) | PLL_CLKOD_SET(no), \
	.pllcon1	= PLL_CLKF_SET(nf), \
	.pllcon2	= PLL_CLK_BWADJ_SET(nf >> 1), \
	.aclk_div	= CLK_DIV_##_axi_div, \
	.hclk_div	= CLK_DIV_##_ahb_div, \
	.pclk_div	= CLK_DIV_##_apb_div, \
	.rst_dly	= ((nr*500)/24+1), \
}

#define _DPLL_SET_CLKS(khz, nr, nf, no, _ddr_div) \
{ \
	.rate		= khz * KHZ, \
	.pllcon0	= PLL_CLKR_SET(nr) | PLL_CLKOD_SET(no), \
	.pllcon1	= PLL_CLKF_SET(nf), \
	.pllcon2	= PLL_CLK_BWADJ_SET(nf >> 1), \
	.core_div	= CLK_DIV_##_ddr_div, \
	.rst_dly	= ((nr*500)/24+1), \
}


#define _CPLL_SET_CLKS(khz, nr, nf, no) \
{ \
	.rate		= khz * KHZ, \
	.pllcon0	= PLL_CLKR_SET(nr) | PLL_CLKOD_SET(no), \
	.pllcon1	= PLL_CLKF_SET(nf), \
	.pllcon2	= PLL_CLK_BWADJ_SET(nf >> 1), \
	.rst_dly	= ((nr*500)/24+1), \
}

struct pll_data {
	u32 id;
	u32 size;
	struct pll_clk_set *clkset;
};

#define SET_PLL_DATA(_pll_id, _table, _size) \
{\
	.id = (u32)(_pll_id), \
	.size = (u32)(_size), \
	.clkset = (struct pll_clk_set *)(_table), \
}



/* 		rk3288 pll notice 		*/
/* 
 * Fref = Fin / nr
 * Fvco = Fin * nf / nr
 * Fout = Fvco / no
 *
 * Fin value range requirement:		32KHz ~ 2200MHz
 * Fref value range requirement:	32KHz ~ 50MHz
 * Fvco value range requirement:	1100MHz ~ 2200MHz
 * Fout value range requirement:	30MHz ~ 2200MHz
 *
 */

/* apll clock table, should be from high to low */
static const struct pll_clk_set apll_clks[] = {
	//rate, nr, nf, no,		a12_div, mp_div, m0_div,	l2, atclk, pclk_dbg
	_APLL_SET_CLKS(1008000,1, 84, 2,	1, 4, 2,		2, 4, 4),
	_APLL_SET_CLKS(912000, 1, 76, 2,	1, 4, 2,		2, 4, 4),
	_APLL_SET_CLKS(816000, 1, 68, 2,	1, 4, 2,		2, 4, 4),
	_APLL_SET_CLKS(600000, 1, 50, 2,	1, 4, 2,		2, 4, 4),
};


/* gpll clock table, should be from high to low */
static const struct pll_clk_set gpll_clks[] = {
	//rate, nr, nf, no,		aclk_div, hclk_div, pclk_div
	_GPLL_SET_CLKS(768000, 1,  64, 2,    4, 2, 4),
	_GPLL_SET_CLKS(594000, 2, 198, 4,    4, 1, 2),
	_GPLL_SET_CLKS(300000, 1,  50, 4,    2, 1, 2),
	_GPLL_SET_CLKS(297000, 2, 198, 8,    2, 1, 2),
	_GPLL_SET_CLKS(148500, 2,  99, 8,    1, 2, 4),
};


/* cpll clock table, should be from high to low */
static const struct pll_clk_set cpll_clks[] = {
	//rate, nr, nf, no
	_CPLL_SET_CLKS(798000, 2, 133, 2),
	_CPLL_SET_CLKS(594000, 2, 198, 4),
};


struct pll_data rkpll_data[END_PLL_ID] = {
	SET_PLL_DATA(APLL_ID, apll_clks, ARRAY_SIZE(apll_clks)),
	SET_PLL_DATA(DPLL_ID, NULL, 0),
	SET_PLL_DATA(CPLL_ID, cpll_clks, ARRAY_SIZE(cpll_clks)),
	SET_PLL_DATA(GPLL_ID, gpll_clks, ARRAY_SIZE(gpll_clks)),
};

static void rkclk_pll_wait_lock(enum rk_plls_id pll_id)
{
	#define GRF_SOC_STATUS1		0x0284
	uint32_t pll_state[END_PLL_ID] = {1, 0, 2, 3, 4};
	uint32_t bit = (0x20u << pll_state[pll_id]);

	/* delay for pll lock */
	while (1) {
		if (readl((const void *)(REG_FILE_BASE_ADDR + GRF_SOC_STATUS1)) & bit) {
			break;
		}
		clk_loop_delayus(1);
	}
}


static int rkclk_pll_clk_set_rate(enum rk_plls_id pll_id, uint32_t mHz, pll_callback_f cb_f)
{
	struct pll_data *pll = NULL;
	struct pll_clk_set *clkset = NULL;
	unsigned long rate = mHz * MHZ;
	int i = 0;

	for (i=0; i<END_PLL_ID; i++) {
		if (rkpll_data[i].id == pll_id) {
			pll = &rkpll_data[i];
			break;
		}
	}

	if ((pll == NULL) || (pll->clkset == NULL)) {
		return -1;
	}

	for (i=0; i<pll->size; i++) {
		if (pll->clkset[i].rate <= rate) {
			clkset = &(pll->clkset[i]);
			break;
		}
	}

	if (clkset == NULL) {
		return -1;
	}

	/* PLL enter slow-mode */
	cru_writel(PLL_MODE_SLOW(pll_id), CRU_MODE_CON);

	/* enter rest */
	cru_writel((PLL_RESET | PLL_RESET_W_MSK), PLL_CONS(pll_id, 3));

	cru_writel(clkset->pllcon0, PLL_CONS(pll_id, 0));
	cru_writel(clkset->pllcon1, PLL_CONS(pll_id, 1));
	cru_writel(clkset->pllcon2, PLL_CONS(pll_id, 2));

	clk_loop_delayus(10);
	/* return form rest */
	cru_writel(PLL_RESET_RESUME | PLL_RESET_W_MSK, PLL_CONS(pll_id, 3));
	clk_loop_delayus(clkset->rst_dly);

	/* waiting for pll lock */
	rkclk_pll_wait_lock(pll_id);

	if (cb_f != NULL) {
		cb_f(clkset);
	}

	/* PLL enter normal-mode */
	cru_writel(PLL_MODE_NORM(pll_id), CRU_MODE_CON);

	return 0;
}


static uint32_t rkclk_pll_clk_get_rate(enum rk_plls_id pll_id)
{
	uint32_t nr, no, nf;
	uint32_t con;

	con = cru_readl(CRU_MODE_CON);
	con = con & PLL_MODE_MSK(pll_id);
	con = con >> (pll_id*4);
	if (con == 0) {
		/* slow mode */
		return (24 * MHZ);
	} else if (con == 1) {
		/* normal mode */
		con = cru_readl(PLL_CONS(pll_id, 0));
		no = PLL_NO(con);
		nr = PLL_NR(con);
		con = cru_readl(PLL_CONS(pll_id, 1));
		nf = PLL_NF(con);

		return (24 * nf / (nr * no)) * MHZ;
	} else {
		/* deep slow mode */
		return 32768;
	}
}


/*
 * rkplat clock set periph clock from general pll
 * 	when call this function, make sure pll is in slow mode
 */
static void rkclk_periph_ahpclk_set(uint32_t pll_src, uint32_t aclk_div, uint32_t hclk_div, uint32_t pclk_div)
{
	uint32_t pll_sel = 0, a_div = 0, h_div = 0, p_div = 0;

	/* periph clock source select: 0: codec pll, 1: general pll */
	if (pll_src == 0) {
		pll_sel = PERI_SEL_CPLL;
	} else {
		pll_sel = PERI_SEL_GPLL;
	}

	/* periph aclk - aclk_periph = periph_clk_src / (peri_aclk_div_con + 1) */
	if (aclk_div == 0) {
		a_div = 1;
	} else {
		a_div = aclk_div - 1;
	}

	/* periph hclk - aclk_bus: hclk_bus = 1:1 or 2:1 or 4:1 */
	switch (hclk_div)
	{
		case CLK_DIV_1:
			h_div = 0;
			break;
		case CLK_DIV_2:
			h_div = 1;
			break;
		case CLK_DIV_4:
			h_div = 2;
			break;
		default:
			h_div = 1;
			break;
	}

	/* periph pclk - aclk_bus: pclk_bus = 1:1 or 2:1 or 4:1 or 8:1 */
	switch (pclk_div)
	{
		case CLK_DIV_1:
			p_div = 0;
			break;
		case CLK_DIV_2:
			p_div = 1;
			break;
		case CLK_DIV_4:
			p_div = 2;
			break;
		case CLK_DIV_8:
			p_div = 3;
			break;
		default:
			p_div = 2;
			break;
	}

	cru_writel((PERI_SEL_PLL_W_MSK | pll_sel)
			| (PERI_PCLK_DIV_W_MSK | (p_div << PERI_PCLK_DIV_OFF))
			| (PERI_HCLK_DIV_W_MSK | (h_div << PERI_HCLK_DIV_OFF))
			| (PERI_ACLK_DIV_W_MSK | (a_div << PERI_ACLK_DIV_OFF)), CRU_CLKSELS_CON(10));
}


/*
 * rkplat clock set cpu clock from arm pll
 * 	when call this function, make sure pll is in slow mode
 */
static void rkclk_cpu_coreclk_set(uint32_t pll_src, uint32_t a12_core_div, uint32_t aclk_core_mp_div, uint32_t aclk_core_m0_div)
{
	uint32_t pll_sel = 0, a12_div = 0, mp_div = 0, m0_div = 0;

	/* cpu clock source select: 0: arm pll, 1: general pll */
	if (pll_src == 0) {
		pll_sel = CORE_SEL_APLL;
	} else {
		pll_sel = CORE_SEL_GPLL;
	}

	/* a12 core clock div: clk_core = clk_src / (div_con + 1) */
	if (a12_core_div == 0) {
		a12_div = 1;
	} else {
		a12_div = a12_core_div - 1;
	}

	/* mp core axi clock div: clk = clk_src / (div_con + 1) */
	if (aclk_core_mp_div == 0) {
		mp_div = 1;
	} else {
		mp_div = aclk_core_mp_div - 1;
	}

	/* m0 core axi clock div: clk = clk_src / (div_con + 1) */
	if (aclk_core_m0_div == 0) {
		m0_div = 1;
	} else {
		m0_div = aclk_core_m0_div - 1;
	}

	cru_writel((CORE_SEL_PLL_W_MSK | pll_sel)
			| (A12_CORE_CLK_DIV_W_MSK | (a12_div << A12_CORE_CLK_DIV_OFF))
			| (MP_AXI_CLK_DIV_W_MSK | (mp_div << MP_AXI_CLK_DIV_OFF))
			| (M0_AXI_CLK_DIV_W_MSK | (m0_div << M0_AXI_CLK_DIV_OFF)), CRU_CLKSELS_CON(0));
}


/*
 * rkplat clock set l2ram/pclk_dbg/atclk from arm pll
 * 	when call this function, make sure pll is in slow mode
 */
static void rkclk_cpu_l2dbgatclk_set(uint32_t l2ram_div, uint32_t atclk_core_div, uint32_t pclk_core_dbg_div)
{
	uint32_t l2_div = 0, atclk_div = 0, pclk_dbg_div = 0;

	/* l2 clock div: clk = clk_src / (clk_div_con + 1) */
	if (l2ram_div == 0) {
		l2_div = 1;
	} else {
		l2_div = l2ram_div - 1;
	}

	/* atclk clock div: clk = clk_src / (clk_div_con + 1) */
	if (atclk_core_div == 0) {
		atclk_div = 1;
	} else {
		atclk_div = atclk_core_div - 1;
	}

	/* pclk dbg clock div: clk = clk_src / (clk_div_con + 1) */
	if (pclk_core_dbg_div == 0) {
		pclk_dbg_div = 1;
	} else {
		pclk_dbg_div = pclk_core_dbg_div - 1;
	}

	cru_writel(((0x7 << (0 + 16)) | (l2_div << 0))
			| ((0x1f << (4 + 16)) | (atclk_div << 4))
			| ((0x1f << (9 + 16)) | (pclk_dbg_div << 9)), CRU_CLKSELS_CON(37));
}




static void rkclk_apll_cb(struct pll_clk_set *clkset)
{
	rkclk_cpu_coreclk_set(CPU_SRC_ARM_PLL, clkset->a12_core_div, clkset->aclk_core_mp_div, clkset->aclk_core_m0_div);
	rkclk_cpu_l2dbgatclk_set(clkset->l2ram_div, clkset->atclk_core_div, clkset->pclk_dbg_div);
}


static void rkclk_gpll_cb(struct pll_clk_set *clkset)
{
	rkclk_periph_ahpclk_set(PERIPH_SRC_GENERAL_PLL, clkset->aclk_div, clkset->hclk_div, clkset->pclk_div);
}

#if 0
static void rkclk_dpll_cb(struct pll_clk_set *clkset)
{
	rkclk_ddr_clk_set(DDR_SRC_DDR_PLL, clkset->a12_core_div);
}

/*
 * rkplat clock set bus clock from general pll
 * 	when call this function, make sure pll is in slow mode
 */
static void rkclk_bus_ahpclk_set(uint32_t pll_src, uint32_t axi_div, uint32_t aclk_div, uint32_t hclk_div, uint32_t pclk_div)
{
	uint32_t pll_sel = 0, axi_bus_div = 0, a_div = 0, h_div = 0, p_div = 0;

	/* pd bus clock source select: 0: codec pll, 1: general pll */
	if (pll_src == 0) {
		pll_sel = PDBUS_SEL_CPLL;
	} else {
		pll_sel = PDBUS_SEL_GPLL;
	}

	/* pd bus axi - clk = clk_src / (axi_div_con + 1) */
	if (axi_div == 0) {
		axi_bus_div = 1;
	} else {
		axi_bus_div = axi_div - 1;
	}

	/* pd bus aclk - aclk_pdbus = clk_src / (aclk_div_con + 1) */
	if (aclk_div == 0) {
		a_div = 1;
	} else {
		a_div = aclk_div - 1;
	}

	/* pd bus hclk - aclk_bus: hclk_bus = 1:1 or 2:1 or 4:1 */
	switch (hclk_div)
	{
		case CLK_DIV_1:
			h_div = 0;
			break;
		case CLK_DIV_2:
			h_div = 1;
			break;
		case CLK_DIV_4:
			h_div = 2;
			break;
		default:
			h_div = 1;
			break;
	}

	/* pd bus pclk - pclk_pdbus = clk_src / (pclk_div_con + 1) */
	if (pclk_div == 0) {
		p_div = 1;
	} else {
		p_div = pclk_div - 1;
	}

	cru_writel((PDBUS_SEL_PLL_W_MSK | pll_sel)
			| (PDBUS_PCLK_DIV_W_MSK | (p_div << PDBUS_PCLK_DIV_OFF))
			| (PDBUS_HCLK_DIV_W_MSK | (h_div << PDBUS_HCLK_DIV_OFF))
			| (PDBUS_ACLK_DIV_W_MSK | (a_div << PDBUS_ACLK_DIV_OFF))
			| (PDBUS_AXI_DIV_W_MSK | (axi_bus_div << PDBUS_AXI_DIV_OFF)), CRU_CLKSELS_CON(1));
}



/*
 * rkplat clock set ddr clock from ddr pll
 * 	when call this function, make sure pll is in slow mode
 */
static void rkclk_ddr_clk_set(uint32_t pll_src, uint32_t ddr_div)
{
	uint32_t pll_sel = 0, div = 0;

	/* cpu clock source select: 0: ddr pll, 1: general pll */
	if (pll_src == 0) {
		pll_sel = 0;
	} else {
		pll_sel = 1;
	}

	/* ddrphy clk - clk_ddr_src:clk_ddrphy */
	switch (ddr_div)
	{
		case CLK_DIV_1:
			div = 0;
			break;
		case CLK_DIV_2:
			div = 1;
			break;
		case CLK_DIV_4:
			div = 2;
			break;
		default:
			div = 0;
			break;
	}

	cru_writel((0x01 << (8 + 16) | (pll_sel << 8)) | ((0x03 << (0 + 16)) | (div << 0)), CRU_CLKSELS_CON(26));
}

static uint32_t rkclk_get_cpu_mp_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(0));
	div = ((con & MP_AXI_CLK_DIV_MSK) >> MP_AXI_CLK_DIV_OFF) + 1;

	return div;
}


static uint32_t rkclk_get_cpu_m0_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(0));
	div = ((con & M0_AXI_CLK_DIV_MSK) >> M0_AXI_CLK_DIV_OFF) + 1;

	return div;
}


static uint32_t rkclk_get_cpu_l2ram_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(37));
	div = (con & 0x07) + 1;

	return div;
}


static uint32_t rkclk_get_bus_axi_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(1));
	div = ((con & PDBUS_AXI_DIV_MSK) >> PDBUS_AXI_DIV_OFF) + 1;

	return div;
}


static uint32_t rkclk_get_bus_aclk_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(1));
	div = ((con & PDBUS_ACLK_DIV_MSK) >> PDBUS_ACLK_DIV_OFF) + 1;

	return div;
}


static uint32_t rkclk_get_bus_hclk_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(1));
	switch ((con & PDBUS_HCLK_DIV_MSK) >> PDBUS_HCLK_DIV_OFF)
	{
		case 0:
			div = CLK_DIV_1;
			break;
		case 1:
			div = CLK_DIV_2;
			break;
		case 2:
			div = CLK_DIV_4;
			break;
		default:
			div = CLK_DIV_2;
			break;
	}

	return div;
}

static uint32_t rkclk_get_bus_pclk_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(1));
	switch ((con & PDBUS_PCLK_DIV_MSK) >> PDBUS_PCLK_DIV_OFF)
	{
		case 0:
			div = CLK_DIV_1;
			break;
		case 1:
			div = CLK_DIV_2;
			break;
		case 2:
			div = CLK_DIV_4;
			break;
		case 3:
			div = CLK_DIV_8;
			break;
		default:
			div = CLK_DIV_4;
	}

	return div;
}

static uint32_t rkclk_get_periph_aclk_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(10));
	div = ((con & PERI_ACLK_DIV_MSK) >> PERI_ACLK_DIV_OFF) + 1;

	return div;
}

static uint32_t rkclk_get_periph_hclk_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(10));
	switch ((con & PERI_HCLK_DIV_MSK) >> PERI_HCLK_DIV_OFF)
	{
		case 0:
			div = CLK_DIV_1;
			break;
		case 1:
			div = CLK_DIV_2;
			break;
		case 2:
			div = CLK_DIV_4;
			break;
		default:
			div = CLK_DIV_2;
			break;
	}

	return div;
}


static uint32_t rkclk_get_periph_pclk_div(void)
{
	uint32_t con, div;

	con = cru_readl(CRU_CLKSELS_CON(10));
	switch ((con & PERI_PCLK_DIV_MSK) >> PERI_PCLK_DIV_OFF)
	{
		case 0:
			div = CLK_DIV_1;
			break;
		case 1:
			div = CLK_DIV_2;
			break;
		case 2:
			div = CLK_DIV_4;
			break;
		case 3:
			div = CLK_DIV_8;
			break;
		default:
			div = CLK_DIV_4;
			break;
	}

	return div;
}

#endif

int rk_get_arm_pll(void)
{
	return rkclk_pll_clk_get_rate(APLL_ID);
}

int rk_get_general_pll(void)
{
	return rkclk_pll_clk_get_rate(GPLL_ID);
}

int rk_get_codec_pll(void)
{
	return rkclk_pll_clk_get_rate(CPLL_ID);
}

int rk_get_ddr_pll(void)
{
	return rkclk_pll_clk_get_rate(DPLL_ID);
}

int rk_get_new_pll(void)
{
	return rkclk_pll_clk_get_rate(NPLL_ID);
}


/*
 * rockchip pll clock set
 */
void rkclk_set_pll(void)
{
	rkclk_pll_clk_set_rate(APLL_ID, CONFIG_RKCLK_APLL_FREQ, rkclk_apll_cb);
	rkclk_pll_clk_set_rate(GPLL_ID, CONFIG_RKCLK_GPLL_FREQ, rkclk_gpll_cb);
	rkclk_pll_clk_set_rate(CPLL_ID, CONFIG_RKCLK_CPLL_FREQ, NULL);
}

int rkclk_soft_reset(void)
{
    cru_writel(0x3f<<10 | 0x3f<<26, CRU_SOFTRSTS_CON(2));  //soft reset i2c0 - i2c5
    mdelay(1);
    cru_writel(0x3f<<26, CRU_SOFTRSTS_CON(2));
    return 0;
}
void rkclk_set_ddr_freq(unsigned int MHz)
{
    unsigned int nr,no,nf;
    int delay = 1000;
    unsigned int lock[]={APLL_LOCK,DPLL_LOCK,CPLL_LOCK,GPLL_LOCK};
    unsigned int pll_id = DPLL_ID;
    if(MHz <= 150)
    {
        no = 8;
    }
    else if(MHz <= 200)
    {
        no = 6;
    }
    else if(MHz <= 300)
    {
        no = 4;
    }
    else if(MHz <= 600)
    {
        no = 2;
    }
    else
    {
        no = 1;
    }
    nr = 1;
    nf=(MHz*nr*no)/24;
    
    /* PLL enter slow-mode */
    cru_writel(PLL_MODE_SLOW(pll_id), CRU_MODE_CON);
    //g_cruReg->CRU_PLL_CON[pll_id][3] = PLL_RESET;
    /* enter rest */
    cru_writel((PLL_RESET | PLL_RESET_W_MSK), PLL_CONS(pll_id, 3));
    
    
    cru_writel(NR(nr) | NO(no), PLL_CONS(pll_id, 0));
    cru_writel(NF(nf), PLL_CONS(pll_id, 1));
    cru_writel(NB(nf>>1), PLL_CONS(pll_id, 2));
    
    

    
    udelay(1);
    cru_writel(PLL_RESET_RESUME | PLL_RESET_W_MSK, PLL_CONS(pll_id, 3));
    udelay(1000);
    while (delay > 0) 
    {
        if (g_grfReg->GRF_SOC_STATUS[1] & lock[pll_id])
            break;
        delay--;
    }
	

    /* PLL enter normal-mode */
    cru_writel(PLL_MODE_NORM(pll_id), CRU_MODE_CON);
    
    return 0;
}

