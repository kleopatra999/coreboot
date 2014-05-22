
#ifndef __RK_REG_H
#define __RK_REG_H
#include <stdint.h>

typedef volatile struct tagGICD_REG
{
	u32 ICDDCR		  ;
	u32 ICDICTR		  ;
	u32 ICDIIDR		  ;
	u32 RESERVED0[29]  ;
	u32 ICDISR[4]		;
	u32 RESERVED1[28]  ;
	u32 ICDISER[4]		;
	u32 RESERVED2[28]  ;
	u32 ICDICER[4]	  ;
	u32 RESERVED3[28]  ;
	u32 ICDISPR[4]		;
	u32 RESERVED4[28]  ;
	u32 ICDICPR[4]	  ;
	u32 RESERVED5[28]  ;
	u32 ICDIABR[4]	  ;
	u32 RESERVED6[60]  ;
	u32 ICDIPR_SGI[4]	;
	u32 ICDIPR_PPI[4]	;
	u32 ICDIPR_SPI[18] ;
	u32 RESERVED7[230];
	u32 ITARGETSR[255];
	u32 RESERVED9[1] ;
	u32 ICDICFR[7]	  ;
	u32 RESERVED8[185] ;
	u32 ICDSGIR		  ;
} GICD_REG, *pGICD_REG;
typedef volatile struct tagGICC_REG
{
	u32 ICCICR		 ;
	u32 ICCPMR		 ;
	u32 ICCBPR		 ;
	u32 ICCIAR		 ;
	u32 ICCEOIR		;
	u32 ICCRPR		 ;
	u32 ICCHPIR		;
	u32 ICCABPR		;
	u32 RESERVED0[55];
	u32 ICCIIDR		;
} GICC_REG, *pGICC_REG;


typedef enum _IRQ_NUM{
	INT_DMAC1_0 = 32	   ,
	INT_DMAC1_1 	,
	INT_DMAC2_0 	,
	INT_DMAC2_1 	,
	INT_USB_OTG = 55	,
	INT_USB_Host = 56	,
	INT_USB_Host1 = 57	 ,
	INT_SDMMC = 64		,
	INT_SDIO = 65		,
	INT_SDIO1 = 66		 ,
	INT_eMMC = 67		,
	INT_SARADC = 68 	,
	INT_TSADC = 69	   ,
	INT_NandC = 70		,
	INT_USB_Host0_OHCI	,
	INT_SPI0 = 76		,
	INT_SPI1		,
	INT_SPI2		,
	INT_UART0  = 87 	,
	INT_UART1		,
	INT_UART2		,
	INT_UART3		,
	INT_UART4		,

	INT_I2C0 = 92		,
	INT_I2C1		,
	INT_I2C2		,
	INT_I2C3		,
	INT_I2C4		,
	INT_I2C5		,
	INT_TIMER6CH0 = 98 ,
	INT_TIMER6CH1	,
	INT_TIMER6CH2	,
	INT_TIMER6CH3	,
	INT_TIMER6CH4	,
	INT_TIMER6CH5	,
	INT_TIMER2CH0	,
	INT_TIMER2CH1	,
	INT_GPIO0 = 113 ,
	INT_GPIO1		,
	INT_GPIO2		,
	INT_GPIO3		,
	INT_GPIO4		,
	INT_GPIO5		,
	INT_GPIO6		,
	INT_GPIO7		,
	INT_GPIO8		,
	INT_MAXNUM
} eINT_NUM;

/* PMU registers */
typedef volatile struct tagPMU_REG
{
    u32 PMU_WAKEUP_CFG[2];
    u32 PMU_PWRDN_CON;
    u32 PMU_PWRDN_ST;
    u32 PMU_IDLE_REQ;
    u32 PMU_IDLE_ST;
    u32 PMU_PWRMODE_CON;
    u32 PMU_PWR_STATE;
    u32 PMU_OSC_CNT;
    u32 PMU_PLL_CNT;
    u32 PMU_STABL_CNT;
    u32 PMU_DDR0IO_PWRON_CNT;
    u32 PMU_DDR1IO_PWRON_CNT;
    u32 PMU_CORE_PWRDWN_CNT;
    u32 PMU_CORE_PWRUP_CNT;
    u32 PMU_GPU_PWRDWN_CNT;
    u32 PMU_GPU_PWRUP_CNT;
    u32 PMU_WAKEUP_RST_CLR_CNT;
    u32 PMU_SFT_CON;
    u32 PMU_DDR_SREF_ST;
    u32 PMU_INT_CON;
    u32 PMU_INT_ST;
    u32 PMU_BOOT_ADDR_SEL;
    u32 PMU_GRF_CON;
    u32 PMU_GPIO_SR;
    u32 PMU_GPIO0_A_PULL;
    u32 PMU_GPIO0_B_PULL;
    u32 PMU_GPIO0_C_PULL;
    u32 PMU_GPIO0_A_DRV;
    u32 PMU_GPIO0_B_DRV;
    u32 PMU_GPIO0_C_DRV;
    u32 PMU_GPIO_OP;
    u32 PMU_GPIO0_SEL18;
    u32 PMU_GPIO0_A_IOMUX;
    u32 PMU_GPIO0_B_IOMUX;
    u32 PMU_GPIO0_C_IOMUX;
    u32 PMU_GPIO0_D_IOMUX;
    u32 PMU_PMU_SYS_REG[4];
} PMU_REG, *pPMU_REG;

/*CRU Registers*/
    typedef volatile struct tagCRU_STRUCT
    {
		u32 CRU_PLL_CON[5][4];
		u32 CRU_MODE_CON;
		u32 reserved1[3];
		u32 CRU_CLKSEL_CON[43];
		u32 reserved2[21];
		u32 CRU_CLKGATE_CON[19];
		u32 reserved3[1];
		u32 CRU_GLB_SRST_FST_VALUE;
		u32 CRU_GLB_SRST_SND_VALUE;
		u32 CRU_SOFTRST_CON[12];
		u32 CRU_MISC_CON;
		u32 CRU_GLB_CNT_TH;
		u32 CRU_GLB_RST_CON;
		u32 reserved4[1];
		u32 CRU_GLB_RST_ST;
		u32 reserved5[1];
		u32 CRU_SDMMC_CON[2];
		u32 CRU_SDIO0_CON[2];
		u32 CRU_SDIO1_CON[2];
		u32 CRU_EMMC_CON[2];
    } CRU_REG, *pCRU_REG;



typedef struct tagGPIO_LH{
    u32 GPIOL;
    u32 GPIOH;
} GPIO_LH_T;

typedef struct tagGPIO_PE{
    u32 GPIOA;
    u32 GPIOB;
    u32 GPIOC;
    u32 GPIOD;
} GPIO_PE;
typedef volatile struct tagGRF_REG
{
    u32 reserved[3];
    u32 GRF_GPIO1D_IOMUX;
    u32 GRF_GPIO2A_IOMUX;
    u32 GRF_GPIO2B_IOMUX;
    u32 GRF_GPIO2C_IOMUX;
    u32 reserved2;
    u32 GRF_GPIO3A_IOMUX;
    u32 GRF_GPIO3B_IOMUX;
    u32 GRF_GPIO3C_IOMUX;
    u32 GRF_GPIO3DL_IOMUX;
    u32 GRF_GPIO3DH_IOMUX;
    u32 GRF_GPIO4AL_IOMUX;
    u32 GRF_GPIO4AH_IOMUX;
    u32 GRF_GPIO4BL_IOMUX;
    u32 reserved3;
    u32 GRF_GPIO4C_IOMUX;
    u32 GRF_GPIO4D_IOMUX;
    u32 reserved4;
    u32 GRF_GPIO5B_IOMUX;
    u32 GRF_GPIO5C_IOMUX;
    u32 GRF_GPIO6A_IOMUX;
    u32 reserved5;
    u32 GRF_GPIO6B_IOMUX;
    u32 GRF_GPIO6C_IOMUX;
    u32 reserved6;
    u32 GRF_GPIO7A_IOMUX;
    u32 GRF_GPIO7B_IOMUX;
    u32 GRF_GPIO7CL_IOMU;
    u32 GRF_GPIO7CH_IOMU;
    u32 reserved7;
    u32 GRF_GPIO8A_IOMUX;
    u32 GRF_GPIO8B_IOMUX;
    u32 reserved8[30];
    GPIO_LH_T GRF_GPIO_SR[8];
    GPIO_PE   GRF_GPIO_P[8];
    GPIO_PE   GRF_GPIO_E[8];
    u32 GRF_GPIO_SMT;
    u32 GRF_SOC_CON[15];
    u32 GRF_SOC_STATUS[22];
    u32 GRF_PERIDMAC_CON[4];
    u32 GRF_DDRC0_CON0;
    u32 GRF_DDRC1_CON0;
    u32 GRF_CPU_CON[5];
    u32 reserved9[3];
    u32 GRF_CPU_STATUS0;
    u32 reserved10;
    u32 GRF_UOC0_CON[5];
    u32 GRF_UOC1_CON[5];
    u32 GRF_UOC2_CON[4];
    u32 GRF_UOC3_CON[2];
    u32 GRF_UOC4_CON[2];
    u32 GRF_PVTM_CON[3];
    u32 GRF_PVTM_STATUS[3];
    u32 GRF_IO_VSEL;
    u32 GRF_SARADC_TESTBIT;
    u32 GRF_TSADC_TESTBIT_L;
    u32 GRF_TSADC_TESTBIT_H;
    u32 GRF_OS_REG[4];
    u32 reserved11;
    u32 GRF_SOC_CON15;
    u32 GRF_SOC_CON16;
} GRF_REG, *pGRF_REG;

typedef volatile struct tagSGRF_FILE
{
    u32 SGRF_SOC_CON[6];
    u32 reserved1[(0x20-0x18)/4];
    u32 SGRF_BUSDMAC_CON[2];
    u32 reserved2[(0x40-0x28)/4];
    u32 SGRF_CPU_CON[3];
    u32 reserved3[(0x50-0x4c)/4];
    u32 SGRF_SOC_CON6;
    u32 SGRF_SOC_CON7;
    u32 SGRF_SOC_CON8;
    u32 SGRF_SOC_CON9;
    u32 SGRF_SOC_CON10;
    u32 SGRF_SOC_CON11;
    u32 SGRF_SOC_CON12;
    u32 SGRF_SOC_CON13;
    u32 SGRF_SOC_CON14;
    u32 SGRF_SOC_CON15;
    u32 SGRF_SOC_CON16;
    u32 SGRF_SOC_CON17;
    u32 SGRF_SOC_CON18;
    u32 SGRF_SOC_CON19;
    u32 SGRF_SOC_CON20;
    u32 SGRF_SOC_CON21;
    u32 reserved4[(0x100-0x90)/4];
    u32 SGRF_SOC_STATUS[2];
    u32 reserved5[(0x120-0x108)/4];
    u32 SGRF_FAST_BOOT_ADDR;
} SGRF_REG, *pSGRF_REG;

typedef volatile struct tagTIMER_STRUCT
{
	u32 TIMER_LOAD_COUNT0;
	u32 TIMER_LOAD_COUNT1;
	u32 TIMER_CURR_VALUE0;
	u32 TIMER_CURR_VALUE1;
	u32 TIMER_CTRL_REG;
	u32 TIMER_INT_STATUS;
} TIMER_REG, *pTIMER_REG;

/* SDMMC Host Controller register struct */
typedef volatile struct TagSDC_REG2
{
    volatile u32 SDMMC_CTRL;
    volatile u32 SDMMC_PWREN;
    volatile u32 SDMMC_CLKDIV;
    volatile u32 SDMMC_CLKSRC;
    volatile u32 SDMMC_CLKENA;
    volatile u32 SDMMC_TMOUT;
    volatile u32 SDMMC_CTYPE;
    volatile u32 SDMMC_BLKSIZ;
    volatile u32 SDMMC_BYTCNT;
    volatile u32 SDMMC_INTMASK;
    volatile u32 SDMMC_CMDARG;
    volatile u32 SDMMC_CMD;
    volatile u32 SDMMC_RESP0;
    volatile u32 SDMMC_RESP1;
    volatile u32 SDMMC_RESP2;
    volatile u32 SDMMC_RESP3;
    volatile u32 SDMMC_MINTSTS;
    volatile u32 SDMMC_RINISTS;
    volatile u32 SDMMC_STATUS;
    volatile u32 SDMMC_FIFOTH;
    volatile u32 SDMMC_CDETECT;
    volatile u32 SDMMC_WRTPRT;
    volatile u32 SDMMC_GPIO;
    volatile u32 SDMMC_TCBCNT;
    volatile u32 SDMMC_TBBCNT;
    volatile u32 SDMMC_DEBNCE;
    volatile u32 SDMMC_USRID;
    volatile u32 SDMMC_VERID;
    volatile u32 SDMMC_HCON;
    volatile u32 SDMMC_UHS_REG;
    volatile u32 SDMMC_RST_n;
    volatile u32 SDMMC_CARDTHRCTL;
    volatile u32 SDMMC_BACK_END_POWER;
} SDC_REG_T2, *pSDC_REG_T2;

#define g_Time0Reg	((pTIMER_REG)TIMER0_BASE_ADDR)
#define g_Time7Reg	((pTIMER_REG)TIMER7_BASE_ADDR)

#define g_EMMCReg	((pSDC_REG_T2)EMMC_BASE_ADDR)
#define g_grfReg		((pGRF_REG)GRF_BASE)
#define g_sgrfReg		((pSGRF_REG)GRF_SECURE_BASE)
#define g_cruReg		((pCRU_REG)CRU_BASE_ADDR)

#define	read_XDATA(address)			(*((uint16 volatile *)(address)))
#define	read_XDATA32(address)		(*((u32 volatile *)(address)))
#define	write_XDATA(address, value)	(*((uint16 volatile *)(address)) = value)
#define	write_XDATA32(address, value)	(*((u32 volatile *)(address)) = value)

#endif /* __RK_REG_H */

