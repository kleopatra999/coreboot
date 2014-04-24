/******************************************************************/
/*   Copyright (C) 2007 ROCK-CHIPS FUZHOU . All Rights Reserved.  */
/*******************************************************************
File :  Memap.h
Desc :  memory map address/device base address

Author : huangxinyu
Date : 2007-05-30
Notes :



*********************************************************************/
#ifndef _RK30_MEMMAP_H
#define _RK30_MEMMAP_H

//#define     L2MEM_BASE              0xff700000 // just add temporary , not sure true
#define BOOT_ROM_ADDR           0xffff0000
#define BOOT_ROM_CHIP_VER_ADDR  (BOOT_ROM_ADDR+0x27F0)
#define VOP_BIG_BASE_ADDR  	    0xff930000
#define HDMI_TX_BASE            0xff980000
#define UART0_BASE_ADDR		0xFF180000
#define UART1_BASE_ADDR		0xFF190000
#define UART2_BASE_ADDR		0xFF690000
#define CPU_AXI_BUS_BASE        0xffe00000
#define CORE_GICC_BASE          0xffc02000
#define CORE_GICD_BASE          0xffc01000
#define GIC_CPU_BASE_ADDR       CORE_GICC_BASE
#define GIC_PERI_BASE_ADDR      CORE_GICD_BASE
#define USB_OTG0_BASE_ADDR      0xFF580000
#define USB_OTG_BASE_ADDR	USB_OTG0_BASE_ADDR
#define USB_OTG_BASE_ADDR_VA    USB_OTG_BASE_ADDR
#define USB_HOST_BASE_ADDR      0xFF540000
#define SDMMC0_BASE_ADDR        0xff0d0000
#define SDMMC1_BASE_ADDR        0xff0c0000
#define EMMC_BASE_ADDR          0xff0f0000
#define NANDC0_BASE_ADDR        0xff400000
#define NANDC1_BASE_ADDR        0xff410000
#define NANDC_BASE_ADDR        	NANDC0_BASE_ADDR
#define CRU_BASE_ADDR		0xFF760000
#define GRF_BASE                0xff770000
#define GRF_SECURE_BASE         0xff740000
#define GPIO0_BASE_ADDR         0xFF750000
#define GPIO1_BASE_ADDR         0xFF780000
#define GPIO2_BASE_ADDR         0xFF790000
#define GPIO3_BASE_ADDR         0xFF7A0000
#define GPIO4_BASE_ADDR         0xFF7B0000
#define GPIO5_BASE_ADDR         0xFF7C0000
#define GPIO6_BASE_ADDR         0xFF7D0000
#define GPIO7_BASE_ADDR         0xFF7E0000
#define GPIO8_BASE_ADDR         0xFF7F0000

#define TIMER0_BASE_ADDR        0xFF6B0000
#define TIMER6_BASE_ADDR        0xFF810000
#define eFUSE_BASE_ADDR         0xFFB40000
#define RK_PWM0123_BASE_ADDR    0xFF680000
#define DW_PWM0123_BASE_ADDR    0xFF670000
#define PWM_BASE_ADDR           RK_PWM0123_BASE_ADDR
#define PMU_BASE_ADDR           0xff730000
#define SARADC_BASE_ADDR        0xff100000
#define SARADC_BASE             SARADC_BASE_ADDR

#define REG_FILE_BASE_ADDR	0xFF770000
#define VOP_BIG_BASE_ADDR   	0xff930000
#define DMACS_BUS_BASE_ADDR     0xffb20000
#define DMAC_BUS_BASE_ADDR      0xff600000
#define DMAC_PERI_BASE_ADDR     0xff250000

#define DDR_PCTL0_BASE          0xFF610000
#define DDR_PCTL1_BASE          0xFF630000
#define DDR_PUBL0_BASE          0xFF620000
#define DDR_PUBL1_BASE          0xFF640000
#define DDR_PCTL_BASE           DDR_PCTL0_BASE
#define DDR_PUBL_BASE           DDR_PUBL0_BASE
#define SDRAM_ADDR              0x00000000

#endif /* _HW_MEMMAP_H */



/*********************************************************************
 END OF FILE
*********************************************************************/
