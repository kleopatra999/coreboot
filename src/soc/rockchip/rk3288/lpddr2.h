/********************************************************************************
*********************************************************************************
            COPYRIGHT (c)   2014 BY ROCK-CHIP FUZHOU
                --  ALL RIGHTS RESERVED  --

File Name:      lpddr2.h
Author:         HE CANYANG
Created:        1st Apr 2014
Modified:
Revision:       1.00
********************************************************************************
********************************************************************************/
#ifndef _LPDDR2_HEAD_
#define _LPDDR2_HEAD_

//MR0 (Device Information)
#define  LPDDR2_DAI    (0x1)        // 0:DAI complete, 1:DAI still in progress
#define  LPDDR2_DI     (0x1<<1)     // 0:S2 or S4 SDRAM, 1:NVM
#define  LPDDR2_DNVI   (0x1<<2)     // 0:DNV not supported, 1:DNV supported
#define  LPDDR2_RZQI   (0x3<<3)     // 00:RZQ self test not supported, 01:ZQ-pin may connect to VDDCA or float
                                    // 10:ZQ-pin may short to GND.     11:ZQ-pin self test completed, no error condition detected.

//MR1 (Device Feature)
#define LPDDR2_BL4     (0x2)
#define LPDDR2_BL8     (0x3)
#define LPDDR2_BL16    (0x4)
#define LPDDR2_nWR(n)  (((n)-2)<<5)

//MR2 (Device Feature 2)
#define LPDDR2_RL3_WL1  (0x1)
#define LPDDR2_RL4_WL2  (0x2)
#define LPDDR2_RL5_WL2  (0x3)
#define LPDDR2_RL6_WL3  (0x4)
#define LPDDR2_RL7_WL4  (0x5)
#define LPDDR2_RL8_WL4  (0x6)

//MR3 (IO Configuration 1)
#define LPDDR2_DS_34    (0x1)
#define LPDDR2_DS_40    (0x2)
#define LPDDR2_DS_48    (0x3)
#define LPDDR2_DS_60    (0x4)
#define LPDDR2_DS_80    (0x6)
#define LPDDR2_DS_120   (0x7)   //optional

//MR4 (Device Temperature)
#define LPDDR2_tREF_MASK (0x7)
#define LPDDR2_4_tREF    (0x1)
#define LPDDR2_2_tREF    (0x2)
#define LPDDR2_1_tREF    (0x3)
#define LPDDR2_025_tREF  (0x5)
#define LPDDR2_025_tREF_DERATE    (0x6)

#define LPDDR2_TUF       (0x1<<7)

//MR8 (Basic configuration 4)
#define LPDDR2_S4        (0x0)
#define LPDDR2_S2        (0x1)
#define LPDDR2_N         (0x2)
#define LPDDR2_Density(mr8)  (8<<(((mr8)>>2)&0xf))   // Unit:MB
#define LPDDR2_IO_Width(mr8) (32>>(((mr8)>>6)&0x3))

//MR10 (Calibration)
#define LPDDR2_ZQINIT   (0xFF)
#define LPDDR2_ZQCL     (0xAB)
#define LPDDR2_ZQCS     (0x56)
#define LPDDR2_ZQRESET  (0xC3)

//MR16 (PASR Bank Mask)
// S2 SDRAM Only
#define LPDDR2_PASR_Full (0x0)    
#define LPDDR2_PASR_1_2  (0x1)
#define LPDDR2_PASR_1_4  (0x2)
#define LPDDR2_PASR_1_8  (0x3)

//MR17 (PASR Segment Mask) 1Gb-8Gb S4 SDRAM only

//MR32 (DQ Calibration Pattern A)

//MR40 (DQ Calibration Pattern B)

#endif //_LPDDR2_HEAD_
