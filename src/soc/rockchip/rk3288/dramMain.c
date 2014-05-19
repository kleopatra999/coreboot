/********************************************************************************
*********************************************************************************
            COPYRIGHT (c)   2014 BY ROCK-CHIP FUZHOU
                --  ALL RIGHTS RESERVED  --

File Name:      dramMain.c
Author:         HE CANYANG
Created:        1st Apr 2014
Modified:
Revision:       1.00
********************************************************************************
********************************************************************************/
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
#if 0

/**************************************************************************
* Description: print dram type information
* Parameters : dramType
* Return     : 
* Note       :
***************************************************************************/
void print_ddr_type(unsigned int     dramType)
{
    switch(dramType)
    {
        case LPDDR3:
            printk(BIOS_DEBUG,"LPDDR3 ");
            break;
        case LPDDR2:
            printk(BIOS_DEBUG,"LPDDR2 ");
            break;
        case DDR3:
            printk(BIOS_DEBUG,"DDR3 ");
            break;
        default:
            printk(BIOS_DEBUG,"Unkown Device\n");
            break;
    }
}

/**************************************************************************
* Description: print all dram relational information
* Parameters : void
* Return     : 
* Note       : pChInfo must be initionlazed
***************************************************************************/
void print_ddr_info(void)
{
    unsigned int cap;
    
    printk(BIOS_DEBUG,"Bus Width=");
    print_dec(8<<pChInfo->bw);
    printk(BIOS_DEBUG," ");
    printk(BIOS_DEBUG,"Col=");
    print_dec(pChInfo->col);
    printk(BIOS_DEBUG," ");
    printk(BIOS_DEBUG,"Bank=");
    print_dec(0x1<<(pChInfo->bk));
    printk(BIOS_DEBUG," ");
    printk(BIOS_DEBUG,"Row=");
    print_dec(pChInfo->cs0_row);
    printk(BIOS_DEBUG," ");
    printk(BIOS_DEBUG,"CS=");
    print_dec(pChInfo->cs);
    printk(BIOS_DEBUG," ");
    printk(BIOS_DEBUG,"Die Bus-Width=");
    print_dec(8<<(pChInfo->dBw));
    printk(BIOS_DEBUG," ");
    cap = pChInfo->cs0_cap;
    if(pChInfo->cs>1)
    {
        cap += pChInfo->cs1_cap;
    }
    printk(BIOS_DEBUG,"Size=");
    print_dec(cap);
    printk(BIOS_DEBUG,"MB\n");
}
#endif
/**************************************************************************
* Description: calculate each channel's capability
* Parameters : void
* Return     : 
* Note       : 
***************************************************************************/
void get_ch_cap_info(unsigned int ch)
{
    g_ChInfo[ch].cs0_cap = (1 << (g_ChInfo[ch].cs0_row+g_ChInfo[ch].col+g_ChInfo[ch].bk+g_ChInfo[ch].bw-20));
    if(g_ChInfo[ch].cs > 1)
    {
        g_ChInfo[ch].cs1_cap = (g_ChInfo[ch].cs0_cap) >> (g_ChInfo[ch].cs0_row-g_ChInfo[ch].cs1_row);
    }
    if(g_ChInfo[ch].row_3_4)
    {
        g_ChInfo[ch].cs0_cap = (g_ChInfo[ch].cs0_cap)*3/4;
        g_ChInfo[ch].cs1_cap = (g_ChInfo[ch].cs1_cap)*3/4;
    }
}

/**************************************************************************
* Description: According to g_ChInfo struct, and configuration relational register
* Parameters : global  g_ChInfo struct
* Return     : 
* Note       : 
***************************************************************************/
void dram_all_config(void)
{
    unsigned int ch,chinfo=0;
    unsigned int cap=0,stride=-1;
    unsigned int ch_cap[2]={0,0};
    
    for(ch=0;ch<CHANNEL_CNT;ch++)
    {
        pChInfo = &g_ChInfo[ch];

        switch(pChInfo->dramType)
        {
            case DDR3:
                RECORD_DRAMTYPE_INFO(3);
                break;
            defualt:
                RECORD_DRAMTYPE_INFO(7);
                break;
        }
        RECORD_CS_INFO(ch,pChInfo->cs);
        RECORD_COL_INFO(ch,pChInfo->col);
        RECORD_BK_INFO(ch,pChInfo->bk);
        RECORD_BW_INFO(ch,pChInfo->bw);
        RECORD_DIE_BW_INFO(ch,pChInfo->dBw);
        RECORD_CH_ROW_INFO(ch,pChInfo->row_3_4);
        RECORD_CS0_ROW_INFO(ch,pChInfo->cs0_row);
        RECORD_CS1_ROW_INFO(ch,pChInfo->cs1_row);        
        
        dram_cfg_rbc(pChInfo->cs,pChInfo->cs0_row,pChInfo->cs1_row,pChInfo->bk,pChInfo->col,pChInfo->bw);
        
        ch_cap[ch] = pChInfo->cs0_cap + pChInfo->cs1_cap;
        chinfo |= (1<<ch);
    }
    RECORD_CH_CNT(CHANNEL_CNT);
    RECORD_CH_INFO(chinfo);

    cap = ch_cap[0] + ch_cap[1];
    if(CHANNEL_CNT==1)
    {
        if(chinfo & 1)  //channel a only
        {
            stride = 0x16;
        }
        else //channel b only
        {
            stride = 0x17;
        }
    }
    else // 2 channel
    {
        if(ch_cap[0] == ch_cap[1])
        {
            //interleaved
            switch(cap)
            {
                case 512: // 512MB
                    stride = 0;
                    break;
                case 1024: // 1GB
                    stride = 5;
                    break;
                case 2048: // 2GB
                    stride = 9;
                    break;
                case 4096: // 4GB
                    stride = 0xD;
                    break;
                case 8192: // 8GB
                    stride = 0x1B;
                    break;
            }
        }
        // 3GB special 
        if((ch_cap[0] == 2048) && (ch_cap[1] == 1024))
        {
            stride = 0x10;
        }
        // remain two channel capability not equel OR capability power function of 2
        if(stride == (-1)) 
        {
            if(ch_cap[0] <= ch_cap[1])
            {
                goto error;
            }
            switch(ch_cap[0])
            {
                case 256: // 256MB + 128MB
                    stride = 0;
                    break;
                case 512: // 512MB + 256MB
                    stride = 1;
                    break;
                case 1024:// 1GB + 128MB/256MB/384MB/512MB/768MB
                    stride = 2;
                    break;
                case 2048: // 2GB + 128MB/256MB/384MB/512MB/768MB/1GB/
                    stride = 3;
                    break;
                case 4096: // 4GB + any
                    stride = 0x1a;
                    break;
                default:
                    break;
            }
        }
        if(stride == (-1))
        {
            goto error;
        }
    }
    DDR_STRIDE(stride);
    return;

error:
    printk(BIOS_DEBUG,"Cap not support!\n");
}

/**************************************************************************
* Description: a simple scan to each channel's capability range
* Parameters : void
* Return     : 
* Note       : 
***************************************************************************/
static unsigned int test(void)
{
    unsigned int capTmp,value,addr,ret=0;
    unsigned int cs_cap[2],i;
    unsigned int ch,baseAddr;
    unsigned int stride=DDR_GET_STRIDE();

    for(ch=0;ch<CHANNEL_CNT;ch++)
    {
        pChInfo = &g_ChInfo[ch];

        cs_cap[0]=(1 << (g_ChInfo[ch].cs0_row+g_ChInfo[ch].col+g_ChInfo[ch].bk+g_ChInfo[ch].bw));
        cs_cap[1]=(1 << (g_ChInfo[ch].cs1_row+g_ChInfo[ch].col+g_ChInfo[ch].bk+g_ChInfo[ch].bw));
        baseAddr =pChInfo->baseAddr;
        DDR_STRIDE(ch ? 0x17 : 0x16);
        
        for(i=0;i<pChInfo->cs;i++)
        {
            if(cs_cap[i] < 4)
            {
                ret = -1;
                goto end;
            }
            
            capTmp = cs_cap[i];
            do
            {
                capTmp = capTmp>>1;
                if(!(capTmp & 0x3))
                {
                    writel((baseAddr+capTmp+(i?cs_cap[0]:0)), (baseAddr+capTmp+(i?cs_cap[0]:0)));
                }
            }while(capTmp);

            capTmp = cs_cap[i];
            do
            {
                capTmp = capTmp>>1;
                if(!(capTmp & 0x3))
                {
                    addr = (baseAddr+capTmp+(i?cs_cap[0]:0));
                    value = readl(addr);
                    if(value != addr)
                    {
                        printk(BIOS_DEBUG,"read addr 0x");
                        //print_hex(addr);
                        printk(BIOS_DEBUG," = 0x");
                        //print_hex(value);
                        printk(BIOS_DEBUG,"\n");
                        ret = -1;
                        goto end;
                    }
                }
            }while(capTmp);
        }
    }
    DDR_STRIDE(stride);
    
    return 0;
end:
    return ret;
}

/**************************************************************************
* Description: dram init main entry
* Parameters : 
* Return     : 
* Note       :
***************************************************************************/
unsigned int dram_main(void)
{
    unsigned int ch;
    unsigned int freq;
    unsigned int bSRX=0;

    //used for DDR Test Tool
    if(0xEF08A53C == pPMU_Reg->PMU_PMU_SYS_REG[0])
    {
        goto error;
    }
    
    //increase emmc speed
    g_cruReg->CRU_CLKSEL_CON[12] = ((((0x3F<<8)|(3<<14))<<16) 
                                      | (11<<8)|(1<<14));    // emmc sel GPLL, div 12

    
    printk(BIOS_DEBUG,"DDR Version 1.00 20140512\n");
    printk(BIOS_DEBUG,"In\n");

    if(-1 != dram_is_SelfRefresh())
    {       
        printk(BIOS_DEBUG,"SRX\n");
        bSRX=1;
    }

    freq = 300;
    rkclk_set_ddr_freq(freq);
    ddr_timing_init(freq);
    for(ch=0;ch<CHANNEL_CNT;ch++)
    {
        pChInfo = &g_ChInfo[ch];
        
        //printk(BIOS_DEBUG,"Channel ");
        //printk(BIOS_DEBUG,ch ? "b: " : "a: ");
        //print_ddr_type(pChInfo->dramType);

        if(-1 == dram_init(freq))
        {
            printk(BIOS_DEBUG,"init failed\n");
            goto error;
        }
        get_ch_cap_info(ch);
        //print_ddr_info();
    }
    dram_all_config();    
	
    if(!bSRX)
    {
        if(-1 == test())
        {
            goto error;
        }
    }

end:
    printk(BIOS_DEBUG,"OUT\n"); 
    return 0;

error:
    printk(BIOS_DEBUG,"ERR\n"); 
    return 1;
}

