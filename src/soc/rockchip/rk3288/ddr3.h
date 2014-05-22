
#ifndef CPU_ROCKCHIP_RK3288_DDR3_H
#define CPU_ROCKCHIP_RK3288_DDR3_H

/* mr0 for ddr3 */
#define DDR3_BL8	(0)
#define DDR3_BC4_8	(1)
#define DDR3_BC4	(2)
#define DDR3_CL(n)	(((((n)-4)&0x7)<<4)|((((n)-4)&0x8)>>1))
#define DDR3_WR(n)	(((n)&0x7)<<9)
#define DDR3_DLL_RESET	(1<<8)
#define DDR3_DLL_DeRESET	(0<<8)

/* mr1 for ddr3 */
#define DDR3_DLL_ENABLE	(0)
#define DDR3_DLL_DISABLE	(1)

#define DDR3_MR1_AL(n)	(((n)&0x3)<<3)

#define DDR3_DS_40	(0)
#define DDR3_DS_34	(1<<1)
#define DDR3_Rtt_Nom_DIS	(0)
#define DDR3_Rtt_Nom_60	(1<<2)
#define DDR3_Rtt_Nom_120	(1<<6)
#define DDR3_Rtt_Nom_40	((1<<2)|(1<<6))
#define DDR3_TDQS	(1<<11)
#define DDR3_Qoff	(1<<12)

/* mr2 for ddr3 */
#define DDR3_MR2_CWL(n)	((((n)-5)&0x7)<<3)
#define DDR3_Rtt_WR_DIS	(0)
#define DDR3_Rtt_WR_60	(1<<9)
#define DDR3_Rtt_WR_120	(2<<9)

#endif
