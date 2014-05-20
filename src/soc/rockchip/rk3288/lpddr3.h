/********************************************************************************
*********************************************************************************
            COPYRIGHT (c)   2014 BY ROCK-CHIP FUZHOU
                --  ALL RIGHTS RESERVED  --

File Name:      lpddr3.h
Author:         HE CANYANG
Created:        1st Apr 2014
Modified:
Revision:       1.00
********************************************************************************
********************************************************************************/
#ifndef _LPDDR3_HEAD_
#define _LPDDR3_HEAD_

//MR0 (Device Information)
#define  LPDDR3_DAI    (0x1)        // 0:DAI complete, 1:DAI still in progress
#define  LPDDR3_RZQI   (0x3<<3)     // 00:RZQ self test not supported, 01:ZQ-pin may connect to VDDCA or float
                                    // 10:ZQ-pin may short to GND.     11:ZQ-pin self test completed, no error condition detected.
#define  LPDDR3_WL_SUPOT (1<<6)     // 0:DRAM does not support WL(Set B), 1:DRAM support WL(Set B)
#define  LPDDR3_RL3_SUPOT (1<<7)    // 0:DRAM does not support RL=3,nWR=3,WL=1; 1:DRAM supports RL=3,nWR=3,WL=1 for frequencies <=166

//MR1 (Device Feature)
#define LPDDR3_BL8     (0x3)
#define LPDDR3_nWR(n)  ((n)<<5)

//MR2 (Device Feature 2)
//WL Set A,default
#define LPDDR3_RL3_WL1   (0x1)       // <=166MHz,optional
#define LPDDR3_RL6_WL3   (0x4)       // <=400MHz
#define LPDDR3_RL8_WL4   (0x6)       // <=533MHz
#define LPDDR3_RL9_WL5   (0x7)       // <=600MHz
#define LPDDR3_RL10_WL6  (0x8)       // <=667MHz,default
#define LPDDR3_RL11_WL6  (0x9)       // <=733MHz
#define LPDDR3_RL12_WL6  (0xa)       // <=800MHz
#define LPDDR3_RL14_WL8  (0xc)       // <=933MHz
#define LPDDR3_RL16_WL8  (0xe)       // <=1066MHz
//WL Set B, optional
//#define LPDDR3_RL3_WL1   (0x1)       // <=166MHz,optional
//#define LPDDR3_RL6_WL3   (0x4)       // <=400MHz
//#define LPDDR3_RL8_WL4   (0x6)       // <=533MHz
//#define LPDDR3_RL9_WL5   (0x7)       // <=600MHz
#define LPDDR3_RL10_WL8  (0x8)       // <=667MHz,default
#define LPDDR3_RL11_WL9  (0x9)       // <=733MHz
#define LPDDR3_RL12_WL9  (0xa)       // <=800MHz
#define LPDDR3_RL14_WL11 (0xc)       // <=933MHz
#define LPDDR3_RL16_WL13 (0xe)       // <=1066MHz

#define LPDDR3_nWRE      (1<<4)      // 1:enable nWR programming > 9(defualt)
#define LPDDR3_WL_S      (1<<6)      // 1:Select WL Set B
#define LPDDR3_WR_LEVEL  (1<<7)      // 1:enable

//MR3 (IO Configuration 1)
#define LPDDR3_DS_34    (0x1)
#define LPDDR3_DS_40    (0x2)
#define LPDDR3_DS_48    (0x3)
#define LPDDR3_DS_60    (0x4)        //reserved
#define LPDDR3_DS_80    (0x6)        //reserved
#define LPDDR3_DS_34D_40U   (0x9)  
#define LPDDR3_DS_40D_48U   (0xa)
#define LPDDR3_DS_34D_48U   (0xb)

//MR4 (Device Temperature)
#define LPDDR3_tREF_MASK (0x7)
#define LPDDR3_LT_EXED   (0x0)       // SDRAM Low temperature operating limit exceeded
#define LPDDR3_4_tREF    (0x1)
#define LPDDR3_2_tREF    (0x2)
#define LPDDR3_1_tREF    (0x3)
#define LPDDR3_05_tREF   (0x4)
#define LPDDR3_025_tREF  (0x5)
#define LPDDR3_025_tREF_DERATE    (0x6)
#define LPDDR3_HT_EXED   (0x7)       // SDRAM High temperature operating limit exceeded

#define LPDDR3_TUF       (0x1<<7)    // 1:value has changed since last read of MR4

//MR8 (Basic configuration 4)
#define LPDDR3_S8        (0x3)
#define LPDDR3_Density(mr8)  (8<<(((mr8)>>2)&0xf))   // Unit:MB
#define LPDDR3_IO_Width(mr8) (32>>(((mr8)>>6)&0x3))

//MR10 (Calibration)
#define LPDDR3_ZQINIT   (0xFF)
#define LPDDR3_ZQCL     (0xAB)
#define LPDDR3_ZQCS     (0x56)
#define LPDDR3_ZQRESET  (0xC3)

//MR11 (ODT Control)
#define LPDDR3_ODT_60   (1)           //optional for 1333 and 1600
#define LPDDR3_ODT_120  (2)
#define LPDDR3_ODT_240  (3)
#define LPDDR3_ODT_DIS  (0)

//MR16 (PASR Bank Mask)

//MR17 (PASR Segment Mask) 1Gb-8Gb S4 SDRAM only

//MR32 (DQ Calibration Pattern A)

//MR40 (DQ Calibration Pattern B)

#endif //_LPDDR3_HEAD_
