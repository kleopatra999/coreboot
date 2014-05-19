/********************************************************************************
*********************************************************************************
            COPYRIGHT (c)   2014 BY ROCK-CHIP FUZHOU
                --  ALL RIGHTS RESERVED  --

File Name:      ddr.c
Author:         HE CANYANG
Created:        1st Apr 2014
Modified:
Revision:       1.00
********************************************************************************
********************************************************************************/
#include <types.h>
#include "memap.h"
#include "noc.h"
#include "ddr.h"
#include "ddr3.h"
#include "lpddr2.h"
#include "lpddr3.h"
#include "dramCTL.h"
#include "dramPHY.h"
#include "api_dramCTL.h"



#define __sramdata
#define __sramlocalfunc
#define __sramfunc
#define dsb()  while(0)
#define ddr_delayus  delayus

#define DDR3_DDR2_DLL_DISABLE_FREQ    (250)
#define DDR3_DDR2_ODT_DISABLE_FREQ    (266)

unsigned char __sramdata ddr3_cl_cwl[22][4]={
/*   0~330           330~400         400~533        speed
* tCK  >3             2.5~3          1.875~2.5      1.5~1.875
*    cl<<4, cwl    cl<<4, cwl     cl<<4, cwl              */
    {((5<<4)|5),   ((5<<4)|5),    0          ,   0}, //DDR3_800D
    {((5<<4)|5),   ((6<<4)|5),    0          ,   0}, //DDR3_800E

    {((5<<4)|5),   ((5<<4)|5),    ((6<<4)|6),   0}, //DDR3_1066E
    {((5<<4)|5),   ((6<<4)|5),    ((7<<4)|6),   0}, //DDR3_1066F
    {((5<<4)|5),   ((6<<4)|5),    ((8<<4)|6),   0}, //DDR3_1066G

    {((5<<4)|5),   ((5<<4)|5),    ((6<<4)|6),   ((7<<4)|7)}, //DDR3_1333F
    {((5<<4)|5),   ((5<<4)|5),    ((7<<4)|6),   ((8<<4)|7)}, //DDR3_1333G
    {((5<<4)|5),   ((6<<4)|5),    ((8<<4)|6),   ((9<<4)|7)}, //DDR3_1333H
    {((5<<4)|5),   ((6<<4)|5),    ((8<<4)|6),   ((10<<4)|7)}, //DDR3_1333J

    {((5<<4)|5),   ((5<<4)|5),    ((6<<4)|6),   ((7<<4)|7)}, //DDR3_1600G
    {((5<<4)|5),   ((5<<4)|5),    ((6<<4)|6),   ((8<<4)|7)}, //DDR3_1600H
    {((5<<4)|5),   ((5<<4)|5),    ((7<<4)|6),   ((9<<4)|7)}, //DDR3_1600J
    {((5<<4)|5),   ((6<<4)|5),    ((8<<4)|6),   ((10<<4)|7)}, //DDR3_1600K

    {((5<<4)|5),   ((5<<4)|5),    ((6<<4)|6),   ((8<<4)|7)}, //DDR3_1866J
    {((5<<4)|5),   ((5<<4)|5),    ((7<<4)|6),   ((8<<4)|7)}, //DDR3_1866K
    {((6<<4)|5),   ((6<<4)|5),    ((7<<4)|6),   ((9<<4)|7)}, //DDR3_1866L
    {((6<<4)|5),   ((6<<4)|5),    ((8<<4)|6),   ((10<<4)|7)}, //DDR3_1866M

    {((5<<4)|5),   ((5<<4)|5),    ((6<<4)|6),   ((7<<4)|7)}, //DDR3_2133K
    {((5<<4)|5),   ((5<<4)|5),    ((6<<4)|6),   ((8<<4)|7)}, //DDR3_2133L
    {((5<<4)|5),   ((5<<4)|5),    ((7<<4)|6),   ((9<<4)|7)}, //DDR3_2133M
    {((6<<4)|5),   ((6<<4)|5),    ((7<<4)|6),   ((9<<4)|7)},  //DDR3_2133N

    {((6<<4)|5),   ((6<<4)|5),    ((8<<4)|6),   ((10<<4)|7)} //DDR3_DEFAULT

};
unsigned short int __sramdata ddr3_tRC_tFAW[22]={
/**    tRC    tFAW   */
    ((50<<8)|50), //DDR3_800D
    ((53<<8)|50), //DDR3_800E

    ((49<<8)|50), //DDR3_1066E
    ((51<<8)|50), //DDR3_1066F
    ((53<<8)|50), //DDR3_1066G

    ((47<<8)|45), //DDR3_1333F
    ((48<<8)|45), //DDR3_1333G
    ((50<<8)|45), //DDR3_1333H
    ((51<<8)|45), //DDR3_1333J

    ((45<<8)|40), //DDR3_1600G
    ((47<<8)|40), //DDR3_1600H
    ((48<<8)|40), //DDR3_1600J
    ((49<<8)|40), //DDR3_1600K

    ((45<<8)|35), //DDR3_1866J
    ((46<<8)|35), //DDR3_1866K
    ((47<<8)|35), //DDR3_1866L
    ((48<<8)|35), //DDR3_1866M

    ((44<<8)|35), //DDR3_2133K
    ((45<<8)|35), //DDR3_2133L
    ((46<<8)|35), //DDR3_2133M
    ((47<<8)|35), //DDR3_2133N

    ((53<<8)|50)  //DDR3_DEFAULT
};
static __sramdata unsigned int ddr_speed_bin;    // used for ddr3 only
static __sramdata unsigned int ddr_capability_per_die;  // one chip cs capability

static unsigned int __sramlocalfunc ddr_get_parameter(unsigned int nMHz)
{
    unsigned int tmp;
    unsigned int ret = 0;
    unsigned int al;
    unsigned int bl,bl_tmp;
    unsigned int cl;
    unsigned int cwl;
    PCTL_TIMING_T *p_pctl_timing=&(ddr_timing.pctl_timing);
    PHY_TIMING_T  *p_publ_timing=&(ddr_timing.phy_timing);
    NOC_TIMING_T  *p_noc_timing=&(ddr_timing.noc_timing);
    NOC_ACTIVATE_T  *p_noc_activate=&(ddr_timing.noc_activate);

    p_pctl_timing->togcnt1u = nMHz;
    p_pctl_timing->togcnt100n = nMHz/10;
    p_pctl_timing->tinit = 200;
    p_pctl_timing->trsth = 500;

    #if (CHANNEL_DRAM_TYPE == DDR3)
    {
        //if(ddr_type > DDR3_DEFAULT){
        //    ret = -1;
        //    goto out;
        //}

        #define DDR3_tREFI_7_8_us    (78)  //unit 100ns
        #define DDR3_tMRD            (4)   //tCK
        #define DDR3_tRFC_512Mb      (90)  //ns
        #define DDR3_tRFC_1Gb        (110) //ns
        #define DDR3_tRFC_2Gb        (160) //ns
        #define DDR3_tRFC_4Gb        (300) //ns
        #define DDR3_tRFC_8Gb        (350) //ns
        #define DDR3_tRTW            (2)   //register min valid value
        #define DDR3_tRAS            (37)  //ns
        #define DDR3_tRRD            (10)  //ns
        #define DDR3_tRTP            (7)   //ns
        #define DDR3_tWR             (15)  //ns
        #define DDR3_tWTR            (7)   //ns
        #define DDR3_tXP             (7)   //ns
        #define DDR3_tXPDLL          (24)  //ns
        #define DDR3_tZQCS           (80)  //ns
        #define DDR3_tZQCSI          (0)   //ns
        #define DDR3_tDQS            (1)   //tCK
        #define DDR3_tCKSRE          (10)  //ns
        #define DDR3_tCKE_400MHz     (7)   //ns
        #define DDR3_tCKE_533MHz     (6)   //ns
        #define DDR3_tMOD            (15)  //ns
        #define DDR3_tRSTL           (100) //ns
        #define DDR3_tZQCL           (320) //ns
        #define DDR3_tDLLK           (512) //tCK

        al = 0;
        bl = 8;
        if(nMHz <= 330)
        {
            tmp = 0;
        }
        else if(nMHz<=400)
        {
            tmp = 1;
        }
        else if(nMHz<=533)
        {
            tmp = 2;
        }
        else //666MHz
        {
            tmp = 3;
        }
        if(nMHz < DDR3_DDR2_DLL_DISABLE_FREQ)       //when dll bypss cl = cwl = 6;
        {
            cl = 6;
            cwl = 6;
        }
        else 
        {
            cl = (ddr3_cl_cwl[ddr_speed_bin][tmp] >> 4)&0xf;
            cwl = ddr3_cl_cwl[ddr_speed_bin][tmp] & 0xf;
        }
        if(cl == 0)
            ret = -4;
        if(nMHz <= DDR3_DDR2_ODT_DISABLE_FREQ)
        {
            p_publ_timing->mr[1] = DDR3_DS_40 | DDR3_Rtt_Nom_DIS;
        }
        else
        {
            p_publ_timing->mr[1] = DDR3_DS_40 | DDR3_Rtt_Nom_120;
        }
        if(nMHz <= DDR3_DDR2_DLL_DISABLE_FREQ)
        {
            p_publ_timing->mr[1] |= DDR3_DLL_DISABLE;
        }
        p_publ_timing->mr[2] = DDR3_MR2_CWL(cwl) /* | DDR3_Rtt_WR_60 */;
        p_publ_timing->mr[3] = 0;
        /**************************************************
         * PCTL Timing
         **************************************************/
        /*
         * tREFI, average periodic refresh interval, 7.8us
         */
        p_pctl_timing->trefi = DDR3_tREFI_7_8_us;
        /*
         * tMRD, 4 tCK
         */
        p_pctl_timing->tmrd = DDR3_tMRD & 0x7;
        p_publ_timing->dtpr0.b.tMRD = DDR3_tMRD-4;
        /*
         * tRFC, 90ns(512Mb),110ns(1Gb),160ns(2Gb),300ns(4Gb),350ns(8Gb)
         */
        if(ddr_capability_per_die <= 0x4000000)         // 512Mb 90ns
        {
            tmp = DDR3_tRFC_512Mb;
        }
        else if(ddr_capability_per_die <= 0x8000000)    // 1Gb 110ns
        {
            tmp = DDR3_tRFC_1Gb;
        }
        else if(ddr_capability_per_die <= 0x10000000)   // 2Gb 160ns
        {
            tmp = DDR3_tRFC_2Gb;
        }
        else if(ddr_capability_per_die <= 0x20000000)   // 4Gb 300ns
        {
            tmp = DDR3_tRFC_4Gb;
        }
        else    // 8Gb  350ns
        {
            tmp = DDR3_tRFC_8Gb;
        }
        p_pctl_timing->trfc = (tmp*nMHz+999)/1000;
        p_publ_timing->dtpr1.b.tRFC = ((tmp*nMHz+999)/1000);
        /*
         * tXSR, =tDLLK=512 tCK
         */
        p_pctl_timing->texsr = DDR3_tDLLK;
        p_publ_timing->dtpr2.b.tXS = DDR3_tDLLK;
        /*
         * tRP=CL
         */
        p_pctl_timing->trp = cl;
        p_publ_timing->dtpr0.b.tRP = cl;
        /*
         * WrToMiss=WL*tCK + tWR + tRP + tRCD
         */
        p_noc_timing->b.WrToMiss = (cwl+((DDR3_tWR*nMHz+999)/1000)+cl+cl);
        /*
         * tRC=tRAS+tRP
         */
        p_pctl_timing->trc = ((((ddr3_tRC_tFAW[ddr_speed_bin]>>8)*nMHz+999)/1000)&0x3F);
        p_noc_timing->b.ActToAct = (((ddr3_tRC_tFAW[ddr_speed_bin]>>8)*nMHz+999)/1000);
        p_publ_timing->dtpr0.b.tRC = (((ddr3_tRC_tFAW[ddr_speed_bin]>>8)*nMHz+999)/1000);

        p_pctl_timing->trtw = (cl+2-cwl);//DDR3_tRTW;
        p_publ_timing->dtpr1.b.tRTW = 0;
        p_noc_timing->b.RdToWr = (cl+2-cwl);
        p_pctl_timing->tal = al;
        p_pctl_timing->tcl = cl;
        p_pctl_timing->tcwl = cwl;
        /*
         * tRAS, 37.5ns(400MHz)     37.5ns(533MHz)
         */
        p_pctl_timing->tras = (((DDR3_tRAS*nMHz+(nMHz>>1)+999)/1000)&0x3F);
        p_publ_timing->dtpr0.b.tRAS = ((DDR3_tRAS*nMHz+(nMHz>>1)+999)/1000);
        /*
         * tRCD=CL
         */
        p_pctl_timing->trcd = cl;
        p_publ_timing->dtpr0.b.tRCD = cl;
        /*
         * tRRD = max(4nCK, 7.5ns), DDR3-1066(1K), DDR3-1333(2K), DDR3-1600(2K)
         *        max(4nCK, 10ns), DDR3-800(1K,2K), DDR3-1066(2K)
         *        max(4nCK, 6ns), DDR3-1333(1K), DDR3-1600(1K)
         *
         */
        tmp = ((DDR3_tRRD*nMHz+999)/1000);
        if(tmp < 4)
        {
            tmp = 4;
        }
        p_pctl_timing->trrd = (tmp&0xF);
        p_publ_timing->dtpr0.b.tRRD = tmp;
        p_noc_activate->b.Rrd = tmp;
        /*
         * tRTP, max(4 tCK,7.5ns)
         */
        tmp = ((DDR3_tRTP*nMHz+(nMHz>>1)+999)/1000);
        if(tmp < 4)
        {
            tmp = 4;
        }
        p_pctl_timing->trtp = tmp&0xF;
        p_publ_timing->dtpr0.b.tRTP = tmp;
        /*
         * RdToMiss=tRTP+tRP + tRCD - (BL/2 * tCK)
         */
        p_noc_timing->b.RdToMiss = (tmp+cl+cl-(bl>>1));
        /*
         * tWR, 15ns
         */
        tmp = ((DDR3_tWR*nMHz+999)/1000);
        p_pctl_timing->twr = tmp&0x1F;
        if(tmp<9)
        {
            tmp = tmp - 4;
        }
        else
        {
            tmp += (tmp&0x1) ? 1:0;
            tmp = tmp>>1;
        }
        bl_tmp = (bl == 8) ? DDR3_BL8 : DDR3_BC4;
        p_publ_timing->mr[0] = bl_tmp | DDR3_CL(cl) | DDR3_WR(tmp);

        /*
         * tWTR, max(4 tCK,7.5ns)
         */
        tmp = ((DDR3_tWTR*nMHz+(nMHz>>1)+999)/1000);
        if(tmp < 4)
        {
            tmp = 4;
        }
        p_pctl_timing->twtr = tmp&0xF;
        p_publ_timing->dtpr0.b.tWTR = tmp;
        /*
         * WrToRd=WL+tWTR
         */
        p_noc_timing->b.WrToRd = (tmp+cwl);
        /*
         * tXP, max(3 tCK, 7.5ns)(<933MHz)
         */
        tmp = ((DDR3_tXP*nMHz+(nMHz>>1)+999)/1000);
        if(tmp < 3)
        {
            tmp = 3;
        }
        p_pctl_timing->txp = tmp&0x7;
        /*
         * tXPDLL, max(10 tCK,24ns)
         */
        tmp = ((DDR3_tXPDLL*nMHz+999)/1000);
        if(tmp < 10)
        {
            tmp = 10;
        }
        p_pctl_timing->txpdll = tmp & 0x3F;
        p_publ_timing->dtpr2.b.tXP = tmp;
        /*
         * tZQCS, max(64 tCK, 80ns)
         */
        tmp = ((DDR3_tZQCS*nMHz+999)/1000);
        if(tmp < 64)
        {
            tmp = 64;
        }
        p_pctl_timing->tzqcs = tmp&0x7F;
        /*
         * tZQCSI,
         */
        p_pctl_timing->tzqcsi = DDR3_tZQCSI;
        /*
         * tDQS,
         */
        p_pctl_timing->tdqs = DDR3_tDQS;
        /*
         * tCKSRE, max(5 tCK, 10ns)
         */
        tmp = ((DDR3_tCKSRE*nMHz+999)/1000);
        if(tmp < 5)
        {
            tmp = 5;
        }
        p_pctl_timing->tcksre = tmp & 0x1F;
        /*
         * tCKSRX, max(5 tCK, 10ns)
         */
        p_pctl_timing->tcksrx = tmp & 0x1F;
        /*
         * tCKE, max(3 tCK,7.5ns)(400MHz) max(3 tCK,5.625ns)(533MHz)
         */
        if(nMHz>=533)
        {
            tmp = ((DDR3_tCKE_533MHz*nMHz+999)/1000);
        }
        else
        {
            tmp = ((DDR3_tCKE_400MHz*nMHz+(nMHz>>1)+999)/1000);
        }
        if(tmp < 3)
        {
            tmp = 3;
        }
        p_pctl_timing->tcke = tmp & 0x7;
        /*
         * tCKESR, =tCKE + 1tCK
         */
        p_pctl_timing->tckesr = (tmp+1)&0xF;
        p_publ_timing->dtpr2.b.tCKE = tmp+1;
        /*
         * tMOD, max(12 tCK,15ns)
         */
        tmp = ((DDR3_tMOD*nMHz+999)/1000);
        if(tmp < 12)
        {
            tmp = 12;
        }
        p_pctl_timing->tmod = tmp&0x1F;
        p_publ_timing->dtpr1.b.tMOD = (tmp-12);
        /*
         * tRSTL, 100ns
         */
        p_pctl_timing->trstl = ((DDR3_tRSTL*nMHz+999)/1000)&0x7F;
        /*
         * tZQCL, max(256 tCK, 320ns)
         */
        tmp = ((DDR3_tZQCL*nMHz+999)/1000);
        if(tmp < 256)
        {
            tmp = 256;
        }
        p_pctl_timing->tzqcl = tmp&0x3FF;
        /*
         * tMRR, 0 tCK
         */
        p_pctl_timing->tmrr = 0;
        /*
         * tDPD, 0
         */
        p_pctl_timing->tdpd = 0;

        /**************************************************
         * PHY Timing
         **************************************************/
        /*
         * tCCD, BL/2 for DDR2 and 4 for DDR3
         */
        p_publ_timing->dtpr0.b.tCCD = 0;
        /*
         * tDQSCKmax,5.5ns
         */
        p_publ_timing->dtpr1.b.tDQSCKmax = 0;
        /*
         * tRTODT, 0:ODT may be turned on immediately after read post-amble
         *         1:ODT may not be turned on until one clock after the read post-amble
         */
        p_publ_timing->dtpr1.b.tRTODT = 1;
        /*
         * tFAW,40ns(400MHz 1KB page) 37.5ns(533MHz 1KB page) 50ns(400MHz 2KB page)   50ns(533MHz 2KB page)
         */
        tmp = (((ddr3_tRC_tFAW[ddr_speed_bin]&0x0ff)*nMHz+999)/1000);
        p_publ_timing->dtpr1.b.tFAW = tmp;
        p_noc_activate->b.Fawbank = 1;
        p_noc_activate->b.Faw = tmp;
        /*
         * tAOND_tAOFD
         */
        p_publ_timing->dtpr1.b.tAOND = 0;
        /*
         * tDLLK,512 tCK
         */
        p_publ_timing->dtpr2.b.tDLLK = DDR3_tDLLK;
        /**************************************************
         * NOC Timing
         **************************************************/
        p_noc_timing->b.BurstLen = (bl>>1);
    }
    #else
    #error Channel DRAM type error!!
    #endif
    
    return ret;
}

/**************************************************************************
* Description: calculate dram relational timing, comply with JEDEC
* Parameters : freq: current dram frequency (unit MHz)
* Return     : 
* Note       : we use max speed bin, and max die capability to calculate tRFC
***************************************************************************/
int ddr_timing_init(unsigned int freq)
{
    ddr_speed_bin = 21;
    ddr_capability_per_die = 0x40000000;  // 8Gb

    ddr_get_parameter(freq);
    return 0;
}

