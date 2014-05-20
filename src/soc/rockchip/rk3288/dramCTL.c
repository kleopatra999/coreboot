/********************************************************************************
*********************************************************************************
            COPYRIGHT (c)   2014 BY ROCK-CHIP FUZHOU
                --  ALL RIGHTS RESERVED  --

File Name:      dramCTL.c
Author:         HE CANYANG
Created:        1st Apr 2014
Modified:
Revision:       1.00
********************************************************************************
********************************************************************************/
#define  IN_DRAMCTL
#include <types.h>
#include <console/console.h>
#include <delay.h>
#include "reg.h"
#include "memap.h"
#include "PMU.h"
#include "noc.h"
#include "ddr.h"
#include "ddr3.h"
#include "lpddr2.h"
#include "lpddr3.h"
#include "dramCTL.h"
#include "dramPHY.h"
#include "api_dramCTL.h"
#include "clock.h"

#define select_8bank(n) \
{\
    pPHY_Reg->DCR.b.DDR8BNK = n;\
}

//CS0,n=1
//CS1,n=2
//CS0 & CS1, n=3
#define set_cs(n) \
{\
    pPHY_Reg->PGCR = (pPHY_Reg->PGCR & (~(0xF<<18)))| RANKEN(n);\
}

//bw=2£¬32bit£»bw=1£¬16bit
void set_bw(unsigned int n)
{
    pDDR_REG_T    pDDR_Reg=pChInfo->pDDR_Reg;
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    pMSCH_REG     pMSCH_Reg=pChInfo->pMSCH_Reg;
    
    if(n==1)
    {
        pDDR_Reg->PPCFG |= 1;
        DDR_SET_16BIT(pChInfo->chNum,1);
        pMSCH_Reg->ddrtiming.b.BwRatio = 1;

        pPHY_Reg->DATX8[2].DXGCR &= (~1);
        pPHY_Reg->DATX8[3].DXGCR &= (~1);
        pPHY_Reg->DATX8[2].DXDLLCR |= 0x80000000;  //disable DLL
        pPHY_Reg->DATX8[3].DXDLLCR |= 0x80000000;
    }
    else
    {
        pDDR_Reg->PPCFG &= ~1;
        DDR_SET_16BIT(pChInfo->chNum,0);
        pMSCH_Reg->ddrtiming.b.BwRatio = 0;

        pPHY_Reg->DATX8[2].DXGCR |= 1;
        pPHY_Reg->DATX8[3].DXGCR |= 1;
        if(pDDR_Reg->TOGCNT1U > 250)  //freq > DLL bypass freq
        {
            pPHY_Reg->DATX8[2].DXDLLCR &= ~0x80000000;  //enable DLL
            pPHY_Reg->DATX8[3].DXDLLCR &= ~0x80000000;
        }

        pPHY_Reg->DATX8[2].DXDLLCR &= ~0x40000000;  //reset DLL
        pPHY_Reg->DATX8[3].DXDLLCR &= ~0x40000000;
        udelay(10);
        pPHY_Reg->DATX8[2].DXDLLCR |= 0x40000000;
        pPHY_Reg->DATX8[3].DXDLLCR |= 0x40000000;
    }
    pDDR_Reg->DFISTCFG0 |= (1<<2);
}

void ddr_copy(unsigned int *pDest, unsigned int *pSrc, unsigned int words)
{
    unsigned int i;

    for(i=0; i<words; i++)
    {
        pDest[i] = pSrc[i];
    }
}

/**************************************************************************
* Description: send command to dram device
* Parameters : rank, indicate target command send to
                 rank=1, send command to CS0 device
                 rank=2, send command to CS1 device
                 rank=3, send command to CS0&CS1 device
               cmd, command eg:REF_cmd
               arg, command argument
                 if command is MRS or MRR, and dram type is DDR3, 
                 this arg it composed of bank_addr and cmd_addr macro define
                 if command is MRS or MRR, and dram type is LPDDR2/LPDDR3, 
                 this arg it composed of lpddr2_ma and lpddr2_op macro define               
* Return     : 
* Note       :
***************************************************************************/
void send_command(unsigned int rank, unsigned int cmd, unsigned int arg)
{
    pDDR_REG_T    pDDR_Reg=pChInfo->pDDR_Reg;
    unsigned int i;
    
    pDDR_Reg->MCMD = (start_cmd | (rank<<20) | arg | cmd);
    for (i = 0; i < 10; i ++) {;}
    while(pDDR_Reg->MCMD & start_cmd);
}

static void dfi_cfg(void)
{
    unsigned int  dramType=pChInfo->dramType;
    pDDR_REG_T    pDDR_Reg=pChInfo->pDDR_Reg;
    
    pDDR_Reg->DFISTCFG0 = 0x00000001;
    pDDR_Reg->DFISTCFG1 = 0x00000003;
    pDDR_Reg->DFISTCFG2 = 0x00000003;
    pDDR_Reg->DFILPCFG0 = 0x00070101;

    pDDR_Reg->DFITCTRLDELAY  = 0x00000002;
    pDDR_Reg->DFITPHYWRDATA  = 0x00000001;
    pDDR_Reg->DFITPHYRDLAT   = 0x0000000f;   
    pDDR_Reg->DFITDRAMCLKDIS = 0x00000002;
    pDDR_Reg->DFITDRAMCLKEN  = 0x00000002;    
    pDDR_Reg->DFITPHYUPDTYPE0 = 0x1;

    if(dramType == DDR3)
    {
        pDDR_Reg->DFIODTCFG = (1<<3)|(1<<11);  //cs0 and cs1 write odt enable
        pDDR_Reg->DFIODTCFG1 = (7<<16);        //odt write length 
    }
    else
    {
        pDDR_Reg->DFIODTCFG = 0;
        pDDR_Reg->DFIODTCFG1 = 0;
        pDDR_Reg->DFIODTRANKMAP = 0;
    }
    pDDR_Reg->DFIUPDCFG = 0;          
}

static void pctl_cfg(void) 
{
    unsigned int burstlen;
    unsigned int bl_mddr_lpddr2;
    unsigned int bl_ddr2_ddr3;
    unsigned int bDDR3,bLPDDR3;
    unsigned int dramType=pChInfo->dramType;
    pDDR_REG_T    pDDR_Reg=pChInfo->pDDR_Reg;

    burstlen = ddr_timing.noc_timing.b.BurstLen<<1;
    switch(burstlen)
    {
        case 2:
            bl_mddr_lpddr2 = mddr_lpddr2_bl_2;
            bl_ddr2_ddr3 = ddr2_ddr3_bl_8;
            break;
        case 4:
            bl_mddr_lpddr2 = mddr_lpddr2_bl_4;
            bl_ddr2_ddr3 = ddr2_ddr3_bl_4;
            break;
        case 8:
            bl_mddr_lpddr2 = mddr_lpddr2_bl_8;
            bl_ddr2_ddr3 = ddr2_ddr3_bl_8;
            break;
        case 16:
            bl_mddr_lpddr2 = mddr_lpddr2_bl_16;
            bl_ddr2_ddr3 = ddr2_ddr3_bl_8;
            break;
        default:
            bl_mddr_lpddr2 = mddr_lpddr2_bl_4;
            bl_ddr2_ddr3 = ddr2_ddr3_bl_8;
            break;
    }
    ddr_copy((unsigned int *)&(pDDR_Reg->TOGCNT1U), (unsigned int*)&(ddr_timing.pctl_timing.togcnt1u), 34);

    bDDR3 = 0;
    bLPDDR3 = 0;
    switch(dramType)
    {
        case DDR3:
            if(ddr_timing.phy_timing.mr[1] & DDR3_DLL_DISABLE)
            {
                pDDR_Reg->DFITRDDATAEN   = pDDR_Reg->TCL-3;
            }
            else
            {
                pDDR_Reg->DFITRDDATAEN   = pDDR_Reg->TCL-2;
            }
            pDDR_Reg->DFITPHYWRLAT   = pDDR_Reg->TCWL-1;
            pDDR_Reg->TRSTH = 500;
            pDDR_Reg->MCFG = mddr_lpddr2_clk_stop_idle(0) |  ddr3_en | bl_ddr2_ddr3 | tfaw_cfg(5) | pd_exit_slow | pd_type(1) | pd_idle(0); 
            bDDR3 = 1;
            break;
    }
    DDR_SET_DDR3_TYPE(pChInfo->chNum,bDDR3);
    DDR_SET_LPDDR3_TYPE(pChInfo->chNum,bLPDDR3);
    pDDR_Reg->SCFG.b.hw_low_power_en = 1;
}

void phy_dll_bypass_set(unsigned int freq)
{
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    
    if(freq<=150)
    {
        pPHY_Reg->DLLGCR &= ~(1<<23);
        pPHY_Reg->ACDLLCR |= 0x80000000;
        pPHY_Reg->DATX8[0].DXDLLCR |= 0x80000000;
        pPHY_Reg->DATX8[1].DXDLLCR |= 0x80000000;
        pPHY_Reg->DATX8[2].DXDLLCR |= 0x80000000;
        pPHY_Reg->DATX8[3].DXDLLCR |= 0x80000000;
        pPHY_Reg->PIR |= DLLBYP;
    }
    else if(freq<=250)
    {
        pPHY_Reg->DLLGCR |= (1<<23);
        pPHY_Reg->ACDLLCR |= 0x80000000;
        pPHY_Reg->DATX8[0].DXDLLCR |= 0x80000000;
        pPHY_Reg->DATX8[1].DXDLLCR |= 0x80000000;
        pPHY_Reg->DATX8[2].DXDLLCR |= 0x80000000;
        pPHY_Reg->DATX8[3].DXDLLCR |= 0x80000000;
        pPHY_Reg->PIR |= DLLBYP;
    }
    else
    {
        pPHY_Reg->DLLGCR &= ~(1<<23);
        pPHY_Reg->ACDLLCR &= ~0x80000000;
        pPHY_Reg->DATX8[0].DXDLLCR &= ~0x80000000;
        pPHY_Reg->DATX8[1].DXDLLCR &= ~0x80000000;
        pPHY_Reg->DATX8[2].DXDLLCR &= ~0x80000000;
        pPHY_Reg->DATX8[3].DXDLLCR &= ~0x80000000;
        pPHY_Reg->PIR &= ~DLLBYP;
    }
}

/**************************************************************************
* Description: trigger phy to do a data trainning action
* Parameters : 
* Return     : data trainning fail, return -1
               data trainning success, return 0
* Note       :
***************************************************************************/
static unsigned int _data_training(void)
{
    pDDR_REG_T    pDDR_Reg=pChInfo->pDDR_Reg;
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    unsigned int i,j,backup,cs;
    unsigned int ret=0;
    unsigned int value[2]={QSTRN,RVTRN};
    unsigned int loop;

    loop = 2;
    // disable auto refresh
    backup = pDDR_Reg->TREFI;
    pDDR_Reg->TREFI = 0;
    if(pPHY_Reg->DCR.b.DDRMD != 4)  //mem_type != LPDDR2)
    {
        // passive window
        pPHY_Reg->PGCR |= (1<<1);    
    }
    for(j=0;j<loop;j++)
    {
        // trigger QSTRN and RVTRN
        // clear DTDONE status
        pPHY_Reg->PIR |= CLRSR;
        cs = ((pPHY_Reg->PGCR>>18) & 0xF);    
        // trigger DTT
        pPHY_Reg->PIR |= INIT | value[j] | LOCKBYP | ZCALBYP | CLRSR;
        for (i = 0; i < 10; i ++) {;}
        // wait echo byte DTDONE
        while((pPHY_Reg->DATX8[0].DXGSR[0] & cs) != cs);
        while((pPHY_Reg->DATX8[1].DXGSR[0] & cs) != cs);
        if(!(pDDR_Reg->PPCFG & 1))
        {
            while((pPHY_Reg->DATX8[2].DXGSR[0] & cs) != cs);
            while((pPHY_Reg->DATX8[3].DXGSR[0] & cs) != cs);
        }
        if(pPHY_Reg->PGSR & (DTERR|RVERR|RVEIRR))
        {
            ret=1;
            break;
        }
    }
    // send some auto refresh to complement the lost while DTT
    if(cs > 1)
    {
        send_command(cs, REF_cmd, 0);
        send_command(cs, REF_cmd, 0);
        send_command(cs, REF_cmd, 0);
        send_command(cs, REF_cmd, 0);
    }
    else
    {
        send_command(cs, REF_cmd, 0);
        send_command(cs, REF_cmd, 0);
    }  
    if(pPHY_Reg->DCR.b.DDRMD != 4)  //mem_type != LPDDR2)
    {
        // active window
        pPHY_Reg->PGCR &= ~(1<<1);
    }
    // resume auto refresh
    pDDR_Reg->TREFI = backup;

    //if(pPHY_Reg->PGSR & DTERR)
    if(ret)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static void phy_cfg(void)
{
    unsigned int i;
    unsigned int odt;
    unsigned int dramType=pChInfo->dramType;
    pDDR_REG_T    pDDR_Reg=pChInfo->pDDR_Reg;
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    pMSCH_REG     pMSCH_Reg=pChInfo->pMSCH_Reg;
    
    //DDR PHY Timing
    ddr_copy((unsigned int *)&(pPHY_Reg->DTPR[0]), (unsigned int*)&(ddr_timing.phy_timing.dtpr0), 7);
    pMSCH_Reg->ddrtiming.d32 = (pMSCH_Reg->ddrtiming.b.BwRatio) | ddr_timing.noc_timing.d32;
    pMSCH_Reg->readlatency = 0x3F;
    pMSCH_Reg->activate.d32 =  ddr_timing.noc_activate.d32;
    pMSCH_Reg->devtodev = (2<<4)|(2<<2)|1;
    pPHY_Reg->PTR[0] = tITMSRST(8) | tDLLLOCK(2750) | tDLLSRST(27);
    pPHY_Reg->PTR[1] = tDINIT1(213) | tDINIT0(266525);  //tDINIT1=400ns (533MHz), tDINIT0=500us (533MHz)
    pPHY_Reg->PTR[2] = tDINIT3(534) | tDINIT2(106610);  //tDINIT3=1us (533MHz), tDINIT2=200us (533MHz)
    //pPHY_Reg->DSGCR |= FXDLAT;
    //pPHY_Reg->DXCCR |= RVSEL(1);
    odt=0;
    switch(dramType)
    {
        case DDR3:
            pPHY_Reg->PGCR = (pPHY_Reg->PGCR & (~0x1F)) | DFTLMT(0) | DFTCMP(0) | DQSCFG(0) | ITMDMD(0);
            pPHY_Reg->DCR.b.DDRMD = 3;  //ddr3
            if(ddr_timing.phy_timing.mr[1] & 0x244)
            {
                odt=1;
            }
            break;
    }
    if(odt)
    {
        pPHY_Reg->DATX8[0].DXGCR |= (0x3<<9);  //dynamic RTT enable
        pPHY_Reg->DATX8[1].DXGCR |= (0x3<<9);
        pPHY_Reg->DATX8[2].DXGCR |= (0x3<<9);
        pPHY_Reg->DATX8[3].DXGCR |= (0x3<<9);
    }
    else
    {
        pPHY_Reg->DATX8[0].DXGCR &= ~(0x3<<9);  //dynamic RTT disable
        pPHY_Reg->DATX8[1].DXGCR &= ~(0x3<<9);
        pPHY_Reg->DATX8[2].DXGCR &= ~(0x3<<9);
        pPHY_Reg->DATX8[3].DXGCR &= ~(0x3<<9);
    }
}

void phy_init(void)
{
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    unsigned int i;
    
    pPHY_Reg->PIR = INIT | DLLSRST | DLLLOCK | ZCAL | ITMSRST | CLRSR;
    for (i = 0; i < 10; i ++) {;}
    while((pPHY_Reg->PGSR & (IDONE | DLDONE | ZCDONE)) != (IDONE | DLDONE | ZCDONE));
}

void phy_pctrl_reset(void)
{
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    
    CRU_DDRReset(pChInfo->chNum,1,1);
    udelay(10);
    pPHY_Reg->ACDLLCR &= ~0x40000000;
    pPHY_Reg->DATX8[0].DXDLLCR &= ~0x40000000;
    pPHY_Reg->DATX8[1].DXDLLCR &= ~0x40000000;
    pPHY_Reg->DATX8[2].DXDLLCR &= ~0x40000000;
    pPHY_Reg->DATX8[3].DXDLLCR &= ~0x40000000;
    udelay(100);
    pPHY_Reg->ACDLLCR |= 0x40000000;
    pPHY_Reg->DATX8[0].DXDLLCR |= 0x40000000;
    pPHY_Reg->DATX8[1].DXDLLCR |= 0x40000000;
    pPHY_Reg->DATX8[2].DXDLLCR |= 0x40000000;
    pPHY_Reg->DATX8[3].DXDLLCR |= 0x40000000;
    udelay(100);
    CRU_DDRReset(pChInfo->chNum,1,0);
    udelay(100);
    CRU_DDRReset(pChInfo->chNum,0,0);
    udelay(10);
}

void memory_init(void)
{
    unsigned int  dramType=pChInfo->dramType;
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    unsigned int i;
    
    if(dramType == DDR3)
    {
        pPHY_Reg->PIR |= INIT | DRAMRST | DRAMINIT | LOCKBYP | ZCALBYP | CLRSR | ICPC;
    }
    else
    {
        pPHY_Reg->PIR |= INIT | DRAMINIT | LOCKBYP | ZCALBYP | CLRSR | ICPC;
    }
    for (i = 0; i < 10; i ++) {;}
    while((pPHY_Reg->PGSR & (IDONE | DIDONE)) != (IDONE | DIDONE));
}

void move_to_Config_state(void)
{
    pDDR_REG_T    pDDR_Reg=pChInfo->pDDR_Reg;
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    volatile unsigned int value;

    while(1)
    {
        value = pDDR_Reg->STAT.b.ctl_stat;
        if(value == Config)
        {
            break;
        }
        switch(value)
        {
            case Low_power:
                pDDR_Reg->SCTL = WAKEUP_STATE;
                while((pDDR_Reg->STAT.b.ctl_stat) != Access);
                while((pPHY_Reg->PGSR & DLDONE) != DLDONE);  //wait DLL lock
            case Access:
            case Init_mem:
                pDDR_Reg->SCTL = CFG_STATE;
                while((pDDR_Reg->STAT.b.ctl_stat) != Config);
                break;
            default:  //Transitional state
                break;
        }
    }
}

void move_to_Access_state(void)
{
    pDDR_REG_T    pDDR_Reg=pChInfo->pDDR_Reg;
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    volatile unsigned int value;

    while(1)
    {
        value = pDDR_Reg->STAT.b.ctl_stat;
        if((value == Access)
           || ((pDDR_Reg->STAT.b.lp_trig == 1) && ((pDDR_Reg->STAT.b.ctl_stat) == Low_power)))
        {
            break;
        }
        switch(value)
        {
            case Low_power:
                pDDR_Reg->SCTL = WAKEUP_STATE;
                while((pDDR_Reg->STAT.b.ctl_stat) != Access);
                while((pPHY_Reg->PGSR & DLDONE) != DLDONE);  //wait DLL lock
                break;
            case Init_mem:
                pDDR_Reg->SCTL = CFG_STATE;
                while((pDDR_Reg->STAT.b.ctl_stat) != Config);
            case Config:
                pDDR_Reg->SCTL = GO_STATE;
                while(!(((pDDR_Reg->STAT.b.ctl_stat) == Access)
                      || ((pDDR_Reg->STAT.b.lp_trig == 1) && ((pDDR_Reg->STAT.b.ctl_stat) == Low_power))));
                break;
            default:  //Transitional state
                break;
        }
    }
}


/**************************************************************************
* Description: According to channel information, and configuration row/bank/column relational register
* Parameters : 
* Return     : 
* Note       : only use CS0, then cs=1
               use CS0&CS1, then cs=2
               8 bank, then bank = 3
               4 bank, then bank = 2
***************************************************************************/
unsigned int dram_cfg_rbc(unsigned int cs, unsigned int cs0_row, unsigned int cs1_row, unsigned int bank, unsigned int col, unsigned int bw)
{
    unsigned int  dramType=pChInfo->dramType;
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    pMSCH_REG     pMSCH_Reg=pChInfo->pMSCH_Reg;
    unsigned int i,tmp;
    unsigned int ret=0;

    move_to_Config_state();
    // config
    select_8bank((bank==3) ? 1:0);
    col -= (bw==2) ? 0:1;
    //find [row:bank:col]
    tmp=((cs0_row-12)<<4)|(bank<<2)|(col-9);
    for(i=0; i<(sizeof(ddr_cfg_2_rbc)/sizeof(ddr_cfg_2_rbc[0]));i++)
    {
        if(tmp == ddr_cfg_2_rbc[i])
        {
            break;
        }
    }
    if(i>=(sizeof(ddr_cfg_2_rbc)/sizeof(ddr_cfg_2_rbc[0])))
    {
        ret = -1;
        goto end;
    }

finish:
    pMSCH_Reg->ddrconf = i;
end:
    move_to_Access_state();
    return ret;
}

/**************************************************************************
* Description: check whether dram is in self-refresh mode
* Parameters : 
* Return     : in self-refresh mode, return -1
               not in self-refresh mode, return 0
* Note       : 
***************************************************************************/
unsigned int dram_is_SelfRefresh(void)
{
    if(pPMU_Reg->PMU_PMU_SYS_REG[2])
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/**************************************************************************
* Description: dram initialize
* Parameters : 
* Return     : initialize fail, return -1
               initialize success, return 0
* Note       : 
***************************************************************************/
unsigned int dram_init(unsigned int freq)
{
    unsigned int  dramType=pChInfo->dramType;
    pDDR_REG_T    pDDR_Reg=pChInfo->pDDR_Reg;
    pDDRPHY_REG_T pPHY_Reg=pChInfo->pPHY_Reg;
    unsigned int i;
    unsigned int ret = 0;

    phy_pctrl_reset();
    phy_dll_bypass_set(freq);

    
    dfi_cfg();
    pctl_cfg();
    phy_cfg();

    //print_dec(freq);
    //printk(BIOS_DEBUG,"MHz\n");

    phy_init(); 
    pDDR_Reg->POWCTL = power_up_start;
    while(!(pDDR_Reg->POWSTAT & power_up_done));
    send_command(3, Deselect_cmd, 0);
    udelay(1); //tXPR
    send_command(3, PREA_cmd, 0);
retry:
    memory_init();
    move_to_Config_state();
    set_bw(pChInfo->bw);
    set_cs(pChInfo->cs | 1);
    pPHY_Reg->ZQ1CR[0] = (0x1<<28) | (0x2<<15) | (0x2<<10) | (0x19<<5) | 0x19;  //DS=40ohm,ODT=155ohm
    pPHY_Reg->ZQ0CR[0] = (0x1<<28) | (0x2<<15) | (0x2<<10) | (0x19<<5) | 0x19;  //DS=40ohm,ODT=155ohm

    if(-1 == _data_training())
    {
        ret =  -1;
        goto end;
    }
   
    move_to_Access_state();

end:
    return ret;
}

