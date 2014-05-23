#ifndef CPU_ROCKCHIP_RK3288_PMU_H
#define CPU_ROCKCHIP_RK3288_PMU_H

#include <arch/io.h>

typedef volatile struct tagPMU_FILE {
	unsigned int PMU_WAKEUP_CFG[2];
	unsigned int PMU_PWRDN_CON;
	unsigned int PMU_PWRDN_ST;
	unsigned int PMU_IDLE_REQ;
	unsigned int PMU_IDLE_ST;
	unsigned int PMU_PWRMODE_CON;
	unsigned int PMU_PWR_STATE;
	unsigned int PMU_OSC_CNT;
	unsigned int PMU_PLL_CNT;
	unsigned int PMU_STABL_CNT;
	unsigned int PMU_DDR0IO_PWRON_CNT;
	unsigned int PMU_DDR1IO_PWRON_CNT;
	unsigned int PMU_CORE_PWRDN_CNT;
	unsigned int PMU_CORE_PWRUP_CNT;
	unsigned int PMU_GPU_PWRDWN_CNT;
	unsigned int PMU_GPU_PWRUP_CNT;
	unsigned int PMU_WAKEUP_RST_CLR_CNT;
	unsigned int PMU_SFT_CON;
	unsigned int PMU_DDR_SREF_ST;
	unsigned int PMU_INT_CON;
	unsigned int PMU_INT_ST;
	unsigned int PMU_BOOT_ADDR_SEL;
	unsigned int PMU_GRF_CON;
	unsigned int PMU_GPIO_SR;
	unsigned int PMU_GPIO0_A_PULL;
	unsigned int PMU_GPIO0_B_PULL;
	unsigned int PMU_GPIO0_C_PULL;
	unsigned int PMU_GPIO0_A_DRV;
	unsigned int PMU_GPIO0_B_DRV;
	unsigned int PMU_GPIO0_C_DRV;
	unsigned int PMU_GPIO_OP;
	unsigned int PMU_GPIO0_SEL18;
	unsigned int PMU_GPIO0_A_IOMUX;
	unsigned int PMU_GPIO0_B_IOMUX;
	unsigned int PMU_GPIO0_C_IOMUX;
	unsigned int PMU_GPIO0_D_IOMUX;
	unsigned int PMU_PMU_SYS_REG[4];
} PMU_FILE, *pPMU_FILE;

#define pPMU_Reg ((pPMU_FILE)PMU_BASE_ADDR)

#define MaskRegBits32(addr, y, z)           writel((readl((void *)addr) \
	&~(y))|(z), (void *)addr)

#define RECORD_CH_CNT(n)   MaskRegBits32(&(pPMU_Reg->PMU_PMU_SYS_REG[2]), \
	(0x1<<12), (((n)-1)<<12))

#define RECORD_CH_INFO(ch)  MaskRegBits32(&(pPMU_Reg->PMU_PMU_SYS_REG[2]), \
	(0x3<<28), ((ch)<<28))
/* row_3_4:0=normal, 1=6Gb or 12Gb */
#define RECORD_CH_ROW_INFO(ch, row_3_4) MaskRegBits32( \
	&(pPMU_Reg->PMU_PMU_SYS_REG[2]), (0x1<<(30+(ch))), \
	((row_3_4)<<(30+(ch))))

#define RECORD_DRAMTYPE_INFO(dramType)  MaskRegBits32( \
	&(pPMU_Reg->PMU_PMU_SYS_REG[2]), (0x7<<13), ((dramType)<<13))
/*
 * all ch below,
 * ch=0, channel a;
 * ch=1, channel b;
 */
#define RECORD_CS_INFO(ch, cs)   MaskRegBits32( \
	&(pPMU_Reg->PMU_PMU_SYS_REG[2]), (0x1<<(11+(ch)*16)), \
	(((cs)-1)<<(11+(ch)*16)))

#define RECORD_COL_INFO(ch, col) MaskRegBits32( \
	&(pPMU_Reg->PMU_PMU_SYS_REG[2]), (0x3<<(9+(ch)*16)), \
	(((col)-9)<<(9+(ch)*16)))

#define RECORD_BK_INFO(ch, bk)   MaskRegBits32( \
	&(pPMU_Reg->PMU_PMU_SYS_REG[2]), (0x1<<(8+(ch)*16)), \
	((((bk) == 3) ? 0 : 1)<<(8+(ch)*16)))

#define RECORD_CS0_ROW_INFO(ch, cs0_row) MaskRegBits32( \
	&(pPMU_Reg->PMU_PMU_SYS_REG[2]), (0x3<<(6+(ch)*16)), \
	(((cs0_row)-13)<<(6+(ch)*16)))

#define RECORD_CS1_ROW_INFO(ch, cs1_row) MaskRegBits32( \
	&(pPMU_Reg->PMU_PMU_SYS_REG[2]), (0x3<<(4+(ch)*16)), \
	(((cs1_row)-13)<<(4+(ch)*16)))

#define RECORD_BW_INFO(ch, bw)   MaskRegBits32( \
	&(pPMU_Reg->PMU_PMU_SYS_REG[2]), (0x3<<(2+(ch)*16)), \
	((2>>(bw))<<(2+ch*16)))

#define RECORD_DIE_BW_INFO(ch, die_bw)   MaskRegBits32( \
	&(pPMU_Reg->PMU_PMU_SYS_REG[2]), ((0x3)<<((ch)*16)), \
	((2>>(die_bw))<<((ch)*16)))

#define READ_CH_CNT()         (1+((pPMU_Reg->PMU_PMU_SYS_REG[2]>>12)&0x1))
#define READ_CH_INFO()        ((pPMU_Reg->PMU_PMU_SYS_REG[2]>>28)&0x3)

/* row_3_4:0=normal, 1=6Gb or 12Gb */
#define READ_CH_ROW_INFO(ch)  ((pPMU_Reg->PMU_PMU_SYS_REG[2]>>(30+(ch)))&0x1)

#define READ_DRAMTYPE_INFO()  ((pPMU_Reg->PMU_PMU_SYS_REG[2]>>13)&0x7)
#define READ_CS_INFO(ch)   ((((pPMU_Reg->PMU_PMU_SYS_REG[2])>>(11+(ch)*16)) \
	&0x1)+1)
#define READ_BW_INFO(ch)   (2>>(((pPMU_Reg->PMU_PMU_SYS_REG[2])>>(2+(ch) \
	*16))&0x3))
#define READ_COL_INFO(ch)  (9+(((pPMU_Reg->PMU_PMU_SYS_REG[2])>>(9+(ch) \
	*16))&0x3))
#define READ_BK_INFO(ch)   (3-(((pPMU_Reg->PMU_PMU_SYS_REG[2])>>(8+(ch) \
	*16))&0x1))
#define READ_CS0_ROW_INFO(ch)  (13+(((pPMU_Reg->PMU_PMU_SYS_REG[2])>>(6+(ch) \
	*16))&0x3))
#define READ_CS1_ROW_INFO(ch)  (13+(((pPMU_Reg->PMU_PMU_SYS_REG[2])>>(4+(ch) \
	*16))&0x3))
#define READ_DIE_BW_INFO(ch)   (2>>((pPMU_Reg->PMU_PMU_SYS_REG[2]>>((ch) \
	*16))&0x3))

#endif
