#ifndef CPU_ROCKCHIP_RK3288_DDR_H
#define CPU_ROCKCHIP_RK3288_DDR_H

#define DDR3     (2)
#define DRAM_MAX (0xF)

/* select a dram type */
#define CHANNEL_DRAM_TYPE  (DDR3)
/*
 * used channel count
 * Note:if only use one channel, it must connect to channel a
 */
#define CHANNEL_CNT    (2)

/*
 * Channel a configuration
 * a channel's bus width, 32bit bus=32, 16bit bus=16
 */

#define CHA_BUS_WIDTH  (32)
/* channel a used cs count, use CS0=1, use CS0 and CS1=2 */
#define CHA_CS_CNT     (1)
/* channel a CS0's row number */
#define CHA_CS0_ROW    (15)
/* channel a CS1's row number */
/* Note:if CHA_CS_CNT=1, CHA_CS1_ROW will don't care */
#define CHA_CS1_ROW    (15)
/* channel a bank number, 8bank=8, 4bank=4 */
#define CHA_BANK       (8)
/* channel a column number */
#define CHA_COLUMN     (10)
/*
 * a channel each DRAM die's bus width, 8bit=8, 16bit=16, 32bit=32
 * eg: we use 2pcs *16 DRAM die to compose channel a 32bit bus width
 * then set CHA_EACH_DIE_BUS_WIDTH to 16
 * Note:here, we only care each DRAM die's bus width, no care about
 * how many die used
 */
#define CHA_EACH_DIE_BUS_WIDTH   (16)
/*
 * if channel a each DRAM die is 6Gb or 12Gb, set CHA_EACH_DIE_6Gb_OR_12Gb=1
 * else set CHA_EACH_DIE_6Gb_OR_12Gb=0
 * this is used to support LPDDR2/LPDDR3's 6Gb or 12Gb
 */
#define CHA_EACH_DIE_6Gb_OR_12Gb (0)

/*
 * Channel b configuration
 * b channel's bus width, 32bit bus=32, 16bit bus=16
 */

#define CHB_BUS_WIDTH  (32)
/* channel b used cs count, use CS0=1, use CS0 and CS1=2 */
#define CHB_CS_CNT     (1)
/* channel b CS0's row number */
#define CHB_CS0_ROW    (15)
/* channel b CS1's row number */
/* Note:if CHB_CS_CNT=1, CHB_CS1_ROW will not care */
#define CHB_CS1_ROW    (15)
/* channel b bank number, 8bank=8, 4bank=4 */
#define CHB_BANK       (8)
/* channel b column number */
#define CHB_COLUMN     (10)
/*
 * b channel each DRAM die's bus width, 8bit=8, 16bit=16, 32bit=32
 * eg: we use 2pcs *16 DRAM die to compose channel b 32bit bus width
 * then set CHB_EACH_DIE_BUS_WIDTH to 16
 * Note:here, we only care each DRAM die's bus width, no care about
 * how many die used
 */
#define CHB_EACH_DIE_BUS_WIDTH   (16)
/*
 * if channel b each DRAM die is 6Gb or 12Gb, set CHB_EACH_DIE_6Gb_OR_12Gb=1
 * else set CHB_EACH_DIE_6Gb_OR_12Gb=0
 * this is used to support LPDDR2/LPDDR3's 6Gb or 12Gb
 */
#define CHB_EACH_DIE_6Gb_OR_12Gb (0)

/* GRF_SOC_CON0 */
#define grf_core_idle_req_mode_sel1 (1<<11)
#define grf_core_idle_req_mode_sel0 (1<<10)
#define ddr_16bit_en(ch, n)          (((n)<<(8+(ch))) | ((1<<(8+(ch)))<<16))
#define upctl_c_active_in(ch, n)     (((n)<<(5+(ch))) | ((1<<(5+(ch)))<<16))
#define msch_mainddr3(ch, n)         (((n)<<(3+(ch))) | ((1<<(3+(ch)))<<16))
#define msch_mainpartialpop(ch, n)    (((n)<<(1+(ch))) | ((1<<(1+(ch)))<<16))

/* GRF_SOC_CON2 */
#define ddrpubl_lpddr3_en(ch, n)      (((n)<<(10+(3*(ch)))) \
	| ((1<<(10+(3*(ch))))<<16))
#define ddrupctl_lpddr3_odt_en(ch, n) (((n)<<(9+(3*(ch)))) \
	| ((1<<(9+(3*(ch))))<<16))
#define ddrupctl_bst_disable(ch, n)   (((n)<<(8+(3*(ch)))) \
	| ((1<<(8+(3*(ch))))<<16))


#define DDR_SET_16BIT(ch, n)          do { \
	g_grfReg->GRF_SOC_CON[0] = ddr_16bit_en(ch, n); \
	} while (0)

#define DDR_SET_DDR3_TYPE(ch, n)       do { \
	g_grfReg->GRF_SOC_CON[0] = msch_mainddr3(ch, n); \
	} while (0)

#define DDR_SET_LPDDR3_TYPE(ch, n)     do { \
	g_grfReg->GRF_SOC_CON[2] = ddrpubl_lpddr3_en(ch, n)| \
		ddrupctl_bst_disable(ch, n)|ddrupctl_lpddr3_odt_en(ch, n); \
	} while (0)

#define DDR_SET_LPDDR3_ODT(ch, n)      do { \
	g_grfReg->GRF_SOC_CON[2] = ddrupctl_lpddr3_odt_en(ch, n); \
	} while (0)

#define DDR_STRIDE(n)                 do { \
	g_sgrfReg->SGRF_SOC_CON[2] = (0x1F<<16)|(n); \
	} while (0)

#define DDR_GET_STRIDE()              (g_sgrfReg->SGRF_SOC_CON[2]&0x1F)
/*
 * Description: calculate dram relational timing, comply with JEDEC
 * Parameters : freq: current dram frequency (unit MHz)
 * Return     :
 * Note       : we use max speed bin, and max die capability to calculate tRFC
 */
int ddr_param_init(unsigned int freq);
int ddr_timing_init(unsigned int freq);
#endif
