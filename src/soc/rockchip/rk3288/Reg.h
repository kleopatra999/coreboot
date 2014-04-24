/********************************************************************************
		COPYRIGHT (c)   2013 BY ROCK-CHIP FUZHOU
			--  ALL RIGHTS RESERVED  --
File Name:	
Author:         
Created:        
Modified:
Revision:       1.00
********************************************************************************/
#ifndef __RK_REG_H
#define __RK_REG_H
#include <stdint.h>

typedef volatile struct tagGICD_REG
{
	u32 ICDDCR		  ; 	 //0x000 
	u32 ICDICTR		  ;    //0x004	 
	u32 ICDIIDR		  ;    //0x008
	u32 RESERVED0[29]  ; 
	u32 ICDISR[4]		;	//	 0x080	
	u32 RESERVED1[28]  ;
	u32 ICDISER[4]		;	  // 0x100 
	u32 RESERVED2[28]  ;   
	u32 ICDICER[4]	  ; 	   //0x180	 
	u32 RESERVED3[28]  ;
	u32 ICDISPR[4]		;	   //0x200	
	u32 RESERVED4[28]  ;
	u32 ICDICPR[4]	  ; 	 //0x280   
	u32 RESERVED5[28]  ;
	u32 ICDIABR[4]	  ; 	   //0x300
	u32 RESERVED6[60]  ;
	u32 ICDIPR_SGI[4]	;		// 0x400
	u32 ICDIPR_PPI[4]	;		// 0x410 
	u32 ICDIPR_SPI[18] ; 		//0x420
	u32 RESERVED7[230];
	u32 ITARGETSR[255];		  //0x800
	u32 RESERVED9[1] ;
	u32 ICDICFR[7]	  ; 	   //0xc00
	u32 RESERVED8[185] ;
	u32 ICDSGIR		  ; 	   //0xf00 
}GICD_REG, *pGICD_REG;	
typedef volatile struct tagGICC_REG
{
	u32 ICCICR		 ;		   //0x00 
	u32 ICCPMR		 ;		   //0x04 
	u32 ICCBPR		 ;		   //0x08 
	u32 ICCIAR		 ;		   //0x0c 
	u32 ICCEOIR		;		  //0x10 
	u32 ICCRPR		 ;		   //0x14 
	u32 ICCHPIR		;		  //0x18 
	u32 ICCABPR		;		  //0x1c 
	u32 RESERVED0[55];
	u32 ICCIIDR		;		  //0xfc  
}GICC_REG, *pGICC_REG;


typedef enum _IRQ_NUM
{
	INT_DMAC1_0=32	   ,
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
}eINT_NUM;

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

//CRU Registers
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



typedef struct tagGPIO_LH
{
    u32 GPIOL;
    u32 GPIOH;
} GPIO_LH_T;

typedef struct tagGPIO_PE
{
    u32 GPIOA;
    u32 GPIOB;
    u32 GPIOC;
    u32 GPIOD;
} GPIO_PE;


typedef volatile struct tagGRF_REG
{
    u32 reserved[3];       
    u32 GRF_GPIO1D_IOMUX;   //0X0C
    u32 GRF_GPIO2A_IOMUX;   //0X10
    u32 GRF_GPIO2B_IOMUX;
    u32 GRF_GPIO2C_IOMUX;
    u32 reserved2; 
    u32 GRF_GPIO3A_IOMUX;   //0X20
    u32 GRF_GPIO3B_IOMUX;
    u32 GRF_GPIO3C_IOMUX;
    u32 GRF_GPIO3DL_IOMUX;
    u32 GRF_GPIO3DH_IOMUX; //0X30
    u32 GRF_GPIO4AL_IOMUX;
    u32 GRF_GPIO4AH_IOMUX;
    u32 GRF_GPIO4BL_IOMUX;
    u32 reserved3;           //0X40
    u32 GRF_GPIO4C_IOMUX;
    u32 GRF_GPIO4D_IOMUX;
    u32 reserved4;          
    u32 GRF_GPIO5B_IOMUX;   //0X50
    u32 GRF_GPIO5C_IOMUX;
    u32 GRF_GPIO6A_IOMUX;
    u32 reserved5;  
    u32 GRF_GPIO6B_IOMUX;   //0X60
    u32 GRF_GPIO6C_IOMUX; 
    u32 reserved6;
    u32 GRF_GPIO7A_IOMUX;
    u32 GRF_GPIO7B_IOMUX;  //0X70
    u32 GRF_GPIO7CL_IOMU;
    u32 GRF_GPIO7CH_IOMU;
    u32 reserved7;
    u32 GRF_GPIO8A_IOMUX;  //0X80
    u32 GRF_GPIO8B_IOMUX;   
    u32 reserved8[30];
    GPIO_LH_T GRF_GPIO_SR[8];
    GPIO_PE   GRF_GPIO_P[8];
    GPIO_PE   GRF_GPIO_E[8];
    u32 GRF_GPIO_SMT;     //0x240
    u32 GRF_SOC_CON[15];
    u32 GRF_SOC_STATUS[22];  //0x0280
    u32 GRF_PERIDMAC_CON[4];
    u32 GRF_DDRC0_CON0;       //0x02e0
    u32 GRF_DDRC1_CON0;
    u32 GRF_CPU_CON[5];
    u32 reserved9[3];          
    u32 GRF_CPU_STATUS0;
    u32 reserved10;
    u32 GRF_UOC0_CON[5];  //0x320
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
    u32 reserved11;    //0x3a0
    u32 GRF_SOC_CON15;       
    u32 GRF_SOC_CON16;        
}GRF_REG, *pGRF_REG;


typedef volatile struct tagTIMER_STRUCT
{
	u32 TIMER_LOAD_COUNT0;
	u32 TIMER_LOAD_COUNT1;
	u32 TIMER_CURR_VALUE0;
	u32 TIMER_CURR_VALUE1;
	u32 TIMER_CTRL_REG;
	u32 TIMER_INT_STATUS;
}TIMER_REG,*pTIMER_REG;

/* SDMMC Host Controller register struct */
typedef volatile struct TagSDC_REG2
{
    volatile u32 SDMMC_CTRL;        //SDMMC Control register
    volatile u32 SDMMC_PWREN;       //Power enable register
    volatile u32 SDMMC_CLKDIV;      //Clock divider register
    volatile u32 SDMMC_CLKSRC;      //Clock source register
    volatile u32 SDMMC_CLKENA;      //Clock enable register
    volatile u32 SDMMC_TMOUT;       //Time out register
    volatile u32 SDMMC_CTYPE;       //Card type register
    volatile u32 SDMMC_BLKSIZ;      //Block size register
    volatile u32 SDMMC_BYTCNT;      //Byte count register
    volatile u32 SDMMC_INTMASK;     //Interrupt mask register
    volatile u32 SDMMC_CMDARG;      //Command argument register
    volatile u32 SDMMC_CMD;         //Command register
    volatile u32 SDMMC_RESP0;       //Response 0 register
    volatile u32 SDMMC_RESP1;       //Response 1 register
    volatile u32 SDMMC_RESP2;       //Response 2 register
    volatile u32 SDMMC_RESP3;       //Response 3 register
    volatile u32 SDMMC_MINTSTS;     //Masked interrupt status register
    volatile u32 SDMMC_RINISTS;     //Raw interrupt status register
    volatile u32 SDMMC_STATUS;      //Status register
    volatile u32 SDMMC_FIFOTH;      //FIFO threshold register
    volatile u32 SDMMC_CDETECT;     //Card detect register
    volatile u32 SDMMC_WRTPRT;      //Write protect register
    volatile u32 SDMMC_GPIO;        //GPIO register
    volatile u32 SDMMC_TCBCNT;      //Transferred CIU card byte count
    volatile u32 SDMMC_TBBCNT;      //Transferred host/DMA to/from BIU_FIFO byte count
    volatile u32 SDMMC_DEBNCE;      //Card detect debounce register
    volatile u32 SDMMC_USRID;        //User ID register        
    volatile u32 SDMMC_VERID;        //Synopsys version ID register
    volatile u32 SDMMC_HCON;         //Hardware configuration register          
    volatile u32 SDMMC_UHS_REG;      //UHS-1 register  
    volatile u32 SDMMC_RST_n;        //Hardware reset register
    volatile u32 SDMMC_CARDTHRCTL;   //Card Read Threshold Enable
    volatile u32 SDMMC_BACK_END_POWER; //Back-end Power
}SDC_REG_T2,*pSDC_REG_T2;




#define I2C2_BASE_ADDR          0xFF650000
#define I2C0_BASE_ADDR          0xFF650000
#define I2C1_BASE_ADDR          0xFF140000
#define I2C3_BASE_ADDR          0xFF150000
#define I2C4_BASE_ADDR          0xFF160000
#define I2C5_BASE_ADDR          0xFF170000


#define g_Time0Reg      ((pTIMER_REG)TIMER0_BASE_ADDR)
#define g_EMMCReg       ((pSDC_REG_T2)EMMC_BASE_ADDR)
//#define g_grfReg 		((pGRF_REG)GRF_BASE)

#define 	read_XDATA(address) 			(*((uint16 volatile*)(address)))
#define 	read_XDATA32(address)			(*((u32 volatile*)(address)))
#define 	write_XDATA(address, value) 	(*((uint16 volatile*)(address)) = value)
#define 	write_XDATA32(address, value)	(*((u32 volatile*)(address)) = value)

#endif /* __RK_REG_H */

