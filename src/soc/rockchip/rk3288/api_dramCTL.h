/********************************************************************************
*********************************************************************************
            COPYRIGHT (c)   2014 BY ROCK-CHIP FUZHOU
                --  ALL RIGHTS RESERVED  --

File Name:      api_dramCTL.h
Author:         HE CANYANG
Created:        1st Apr 2014
Modified:
Revision:       1.00
********************************************************************************
********************************************************************************/
#ifndef _API_DRAM_HEAD_
#define _API_DRAM_HEAD_

#define CH_MAX      (2)   //channel count

#define DDR_MR0     (0)
#define DDR_MR1     (1)
#define DDR_MR2     (2)
#define DDR_MR3     (3)

typedef struct PCTRL_TIMING_Tag
{
    //Memory Timing Registers
    unsigned int togcnt1u;               //Toggle Counter 1U Register
    unsigned int tinit;                  //t_init Timing Register
    unsigned int trsth;                  //Reset High Time Register
    unsigned int togcnt100n;             //Toggle Counter 100N Register
    unsigned int trefi;                  //t_refi Timing Register
    unsigned int tmrd;                   //t_mrd Timing Register
    unsigned int trfc;                   //t_rfc Timing Register
    unsigned int trp;                    //t_rp Timing Register
    unsigned int trtw;                   //t_rtw Timing Register
    unsigned int tal;                    //AL Latency Register
    unsigned int tcl;                    //CL Timing Register
    unsigned int tcwl;                   //CWL Register
    unsigned int tras;                   //t_ras Timing Register
    unsigned int trc;                    //t_rc Timing Register
    unsigned int trcd;                   //t_rcd Timing Register
    unsigned int trrd;                   //t_rrd Timing Register
    unsigned int trtp;                   //t_rtp Timing Register
    unsigned int twr;                    //t_wr Timing Register
    unsigned int twtr;                   //t_wtr Timing Register
    unsigned int texsr;                  //t_exsr Timing Register
    unsigned int txp;                    //t_xp Timing Register
    unsigned int txpdll;                 //t_xpdll Timing Register
    unsigned int tzqcs;                  //t_zqcs Timing Register
    unsigned int tzqcsi;                 //t_zqcsi Timing Register
    unsigned int tdqs;                   //t_dqs Timing Register
    unsigned int tcksre;                 //t_cksre Timing Register
    unsigned int tcksrx;                 //t_cksrx Timing Register
    unsigned int tcke;                   //t_cke Timing Register
    unsigned int tmod;                   //t_mod Timing Register
    unsigned int trstl;                  //Reset Low Timing Register
    unsigned int tzqcl;                  //t_zqcl Timing Register
    unsigned int tmrr;                   //t_mrr Timing Register
    unsigned int tckesr;                 //t_ckesr Timing Register
    unsigned int tdpd;                   //t_dpd Timing Register
}PCTL_TIMING_T;

typedef union DTPR_0_Tag
{
    unsigned int d32;
    struct 
    {
        unsigned tMRD : 2;
        unsigned tRTP : 3;
        unsigned tWTR : 3;
        unsigned tRP : 4;
        unsigned tRCD : 4;
        unsigned tRAS : 5;
        unsigned tRRD : 4;
        unsigned tRC : 6;
        unsigned tCCD : 1;
    } b;
}DTPR_0_T;

typedef union DTPR_1_Tag
{
    unsigned int d32;
    struct 
    {
        unsigned tAOND : 2;
        unsigned tRTW : 1;
        unsigned tFAW : 6;
        unsigned tMOD : 2;
        unsigned tRTODT : 1;
        unsigned reserved12_15 : 4;
        unsigned tRFC : 8;
        unsigned tDQSCK : 3;
        unsigned tDQSCKmax : 3;
        unsigned reserved30_31 : 2;
    } b;
}DTPR_1_T;

typedef union DTPR_2_Tag
{
    unsigned int d32;
    struct 
    {
        unsigned tXS : 10;
        unsigned tXP : 5;
        unsigned tCKE : 4;
        unsigned tDLLK : 10;
        unsigned reserved29_31 : 3;
    } b;
}DTPR_2_T;

typedef struct PHY_TIMING_Tag
{
    DTPR_0_T  dtpr0;
    DTPR_1_T  dtpr1;
    DTPR_2_T  dtpr2;
    unsigned int    mr[4];   //LPDDR2 no MR0, mr[2] is mDDR MR1
    unsigned int    mr11;    //for LPDDR3 only
}PHY_TIMING_T;

typedef struct DDR_TIMING_Tag
{
    PCTL_TIMING_T  pctl_timing;
    PHY_TIMING_T   phy_timing;
    NOC_TIMING_T   noc_timing;
    NOC_ACTIVATE_T noc_activate;
}DDR_TIMING_T;

typedef struct CHANNEL_INFO_Tag
{
    //inited data
    unsigned int        chNum;  //channel number,0:channel a; 1:channel b;
    pDDR_REG_T    pDDR_Reg;
    pDDRPHY_REG_T pPHY_Reg;
    pMSCH_REG     pMSCH_Reg;
    //need init data
    unsigned int        dramType;  // =DRAM_MAX, channel invalid
    unsigned int        baseAddr;  //channel base address, reserved
    unsigned int        cs;        //use cs count,1:one cs; 2:two cs
    unsigned int        col;
    unsigned int        bk;        // 3:8bank, 2:4bank
    unsigned int        bw;        //channel buswidth, 2:32bit, 1:16bit, 0:8bit
    unsigned int        dBw;       //die buswidth, 2:32bit, 1:16bit, 0:8bit
    unsigned int        row_3_4;   // 1:6Gb or 12Gb, 0:normal
    unsigned int        cs0_row;
    unsigned int        cs0_cap;   //unit MB
    unsigned int        cs1_row;   
    unsigned int        cs1_cap;   //unit MB   
}CH_INFO,*pCH_INFO;

#ifdef IN_DRAMCTL
DDR_TIMING_T ddr_timing;
CH_INFO  g_ChInfo[2]=
{
    {
        0,        //chNum
        ((pDDR_REG_T)DDR_PCTL0_BASE),    //pDDR_Reg
        ((pDDRPHY_REG_T)DDR_PUBL0_BASE), //pPHY_Reg
        ((pMSCH_REG)Service_BUS),        //pMSCH_Reg

        CHANNEL_DRAM_TYPE,   //dramType
        0, //baseAddr
        CHA_CS_CNT, //cs
        CHA_COLUMN, //col
        (CHA_BANK/4)+1, //bk
        CHA_BUS_WIDTH/16, //bw
        CHA_EACH_DIE_BUS_WIDTH/16, //dBw
        CHA_EACH_DIE_6Gb_OR_12Gb, //row_3_4
        CHA_CS0_ROW, //cs0_row
        0, //cs0_cap
        CHA_CS1_ROW, //cs1_row
        0  //cs1_cap
    },
    {
        1,        //chNum
        ((pDDR_REG_T)DDR_PCTL1_BASE),    //pDDR_Reg
        ((pDDRPHY_REG_T)DDR_PUBL1_BASE), //pPHY_Reg
        ((pMSCH_REG)(Service_BUS+0x80)), //pMSCH_Reg

        CHANNEL_DRAM_TYPE,   //dramType
        0, //baseAddr
        CHB_CS_CNT, //cs
        CHB_COLUMN, //col
        (CHB_BANK/4)+1, //bk
        CHB_BUS_WIDTH/16, //bw
        CHB_EACH_DIE_BUS_WIDTH/16, //dBw
        CHB_EACH_DIE_6Gb_OR_12Gb, //row_3_4
        CHB_CS0_ROW, //cs0_row
        0, //cs0_cap
        CHB_CS1_ROW, //cs1_row
        0  //cs1_cap
    }
};
pCH_INFO  pChInfo;  
#else
extern DDR_TIMING_T ddr_timing;
extern CH_INFO  g_ChInfo[2];
extern pCH_INFO pChInfo;
#endif


/**************************************************************************
* Description: According to channel information, and configuration row/bank/column relational register
* Parameters : 
* Return     : 
* Note       : only use CS0, then cs=1
               use CS0&CS1, then cs=2
               8 bank, then bank = 3
               4 bank, then bank = 2
***************************************************************************/
unsigned int dram_cfg_rbc(unsigned int cs, unsigned int cs0_row, unsigned int cs1_row, unsigned int bank, unsigned int col, unsigned int bw);

/**************************************************************************
* Description: check whether dram is in self-refresh mode
* Parameters : 
* Return     : in self-refresh mode, return FALSE
               not in self-refresh mode, return TRUE
* Note       : 
***************************************************************************/
unsigned int dram_is_SelfRefresh(void);

/**************************************************************************
* Description: dram initialize
* Parameters : 
* Return     : initialize fail, return FALSE
               initialize success, return TRUE
* Note       : 
***************************************************************************/
unsigned int dram_init(unsigned int freq);

#endif //_API_DRAM_HEAD_

