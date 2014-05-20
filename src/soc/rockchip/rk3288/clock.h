/*
 * (C) Copyright 2008-2013 Rockchip Electronics
 * Peter, Software Engineering, <superpeter.cai@gmail.com>.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _RKXX_CLOCK_H
#define _RKXX_CLOCK_H


/* config cpu and general clock in MHZ */
#define KHZ				(1000)
#define MHZ				(1000*1000)

#define NR(n)      ((0x3F<<(8+16)) | (((n)-1)<<8))
#define NO(n)      ((0xF<<16) | ((n)-1))
#define NF(n)      ((0x1FFF<<16) | ((n)-1))
#define NB(n)      ((0xFFF<<16) | ((n)-1))

#define ddrphy_ctl_srstn_req(ch,n) (((0x1<<(4+5*(ch)))<<16) | (n<<(4+5*(ch))))
#define ddrctl_psrstn_req(ch,n)    (((0x1<<(3+5*(ch)))<<16) | (n<<(3+5*(ch))))
#define ddrctl_srstn_req(ch,n)     (((0x1<<(2+5*(ch)))<<16) | (n<<(2+5*(ch))))
#define ddrphy_psrstn_req(ch,n)    (((0x1<<(1+5*(ch)))<<16) | (n<<(1+5*(ch))))
#define ddrphy_srstn_req(ch,n)     (((0x1<<(0+5*(ch)))<<16) | (n<<(0+5*(ch))))


typedef struct PLL_SET_Tag
{
    unsigned short int freq;
    unsigned char  nr;
    unsigned char  no;
    unsigned short int  nf;
}PLL_SET_T,*pPLL_SET_T;

#define CRU_DDRReset(ch,ddrctl, ddrphy)  do{\
    g_cruReg->CRU_SOFTRST_CON[10] = ddrphy_ctl_srstn_req(ch,ddrphy) | ddrctl_psrstn_req(ch,ddrctl) | ddrctl_srstn_req(ch,ddrctl) | ddrphy_psrstn_req(ch,ddrphy) | ddrphy_srstn_req(ch,ddrphy);\
}while(0)
#define CRU_ddrphy_ctl_sReset(ch,n)  do{\
    g_cruReg->CRU_SOFTRST_CON[10] = ddrphy_ctl_srstn_req(ch,n);\
}while(0)
/*
 * rkplat clock set for arm and general pll
 */
void rkclk_set_pll(void);

int rk_get_arm_pll(void);

int rk_get_general_pll(void);

int rk_get_codec_pll(void);

int rk_get_ddr_pll(void);

int rk_get_new_pll(void);

int rkclk_soft_reset(void);

void rkclk_set_ddr_freq(unsigned int MHz);
#endif	/* _RKXX_CLOCK_H */

