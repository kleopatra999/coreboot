#ifndef CPU_ROCKCHIP_RK3288_API_DRAMCTL_H
#define CPU_ROCKCHIP_RK3288_API_DRAMCTL_H

/* channel count */
#define CH_MAX      (2)

#define DDR_MR0     (0)
#define DDR_MR1     (1)
#define DDR_MR2     (2)
#define DDR_MR3     (3)

typedef struct PCTRL_TIMING_Tag {
	/* Memory Timing Registers */
	unsigned int togcnt1u;
	unsigned int tinit;
	unsigned int trsth;
	unsigned int togcnt100n;
	unsigned int trefi;
	unsigned int tmrd;
	unsigned int trfc;
	unsigned int trp;
	unsigned int trtw;
	unsigned int tal;
	unsigned int tcl;
	unsigned int tcwl;
	unsigned int tras;
	unsigned int trc;
	unsigned int trcd;
	unsigned int trrd;
	unsigned int trtp;
	unsigned int twr;
	unsigned int twtr;
	unsigned int texsr;
	unsigned int txp;
	unsigned int txpdll;
	unsigned int tzqcs;
	unsigned int tzqcsi;
	unsigned int tdqs;
	unsigned int tcksre;
	unsigned int tcksrx;
	unsigned int tcke;
	unsigned int tmod;
	unsigned int trstl;
	unsigned int tzqcl;
	unsigned int tmrr;
	unsigned int tckesr;
	unsigned int tdpd;
} PCTL_TIMING_T;

typedef union DTPR_0_Tag {
	unsigned int d32;
	struct {
		unsigned tMRD:2;
		unsigned tRTP:3;
		unsigned tWTR:3;
		unsigned tRP:4;
		unsigned tRCD:4;
		unsigned tRAS:5;
		unsigned tRRD:4;
		unsigned tRC:6;
		unsigned tCCD:1;
	} b;
} DTPR_0_T;

typedef union DTPR_1_Tag {
	unsigned int d32;
	struct {
		unsigned tAOND:2;
		unsigned tRTW:1;
		unsigned tFAW:6;
		unsigned tMOD:2;
		unsigned tRTODT:1;
		unsigned reserved12_15:4;
		unsigned tRFC:8;
		unsigned tDQSCK:3;
		unsigned tDQSCKmax:3;
		unsigned reserved30_31:2;
	} b;
} DTPR_1_T;

typedef union DTPR_2_Tag {
	unsigned int d32;
	struct {
		unsigned tXS:10;
		unsigned tXP:5;
		unsigned tCKE:4;
		unsigned tDLLK:10;
		unsigned reserved29_31:3;
	} b;
} DTPR_2_T;

typedef struct PHY_TIMING_Tag {
	DTPR_0_T  dtpr0;
	DTPR_1_T  dtpr1;
	DTPR_2_T  dtpr2;
	unsigned int    mr[4];
	unsigned int    mr11;
} PHY_TIMING_T;

typedef struct DDR_TIMING_Tag {
	PCTL_TIMING_T  pctl_timing;
	PHY_TIMING_T   phy_timing;
	NOC_TIMING_T   noc_timing;
	NOC_ACTIVATE_T noc_activate;
} DDR_TIMING_T;

typedef struct CHANNEL_INFO_Tag {
	/* channel number,0:channel a; 1:channel b;*/
	unsigned int chNum;
	pDDR_REG_T pDDR_Reg;
	pDDRPHY_REG_T pPHY_Reg;
	pMSCH_REG pMSCH_Reg;
	/* need init data*/
	unsigned int dramType;
	unsigned int baseAddr;
	unsigned int cs;
	unsigned int col;
	unsigned int bk;
	unsigned int bw;
	unsigned int dBw;
	unsigned int row_3_4;
	unsigned int cs0_row;
	unsigned int cs0_cap;
	unsigned int cs1_row;
	unsigned int cs1_cap;
} CH_INFO, *pCH_INFO;

#ifdef IN_DRAMCTL
DDR_TIMING_T ddr_timing;
CH_INFO  g_ChInfo[2] = {
	{
	0,
	((pDDR_REG_T)DDR_PCTL0_BASE),
	((pDDRPHY_REG_T)DDR_PUBL0_BASE),
	((pMSCH_REG)Service_BUS),

	CHANNEL_DRAM_TYPE,
	0,
	CHA_CS_CNT,
	CHA_COLUMN,
	(CHA_BANK/4)+1,
	CHA_BUS_WIDTH/16,
	CHA_EACH_DIE_BUS_WIDTH/16,
	CHA_EACH_DIE_6Gb_OR_12Gb,
	CHA_CS0_ROW,
	0,
	CHA_CS1_ROW,
	0
	},
	{
	1,
	((pDDR_REG_T)DDR_PCTL1_BASE),
	((pDDRPHY_REG_T)DDR_PUBL1_BASE),
	((pMSCH_REG)(Service_BUS+0x80)),
	CHANNEL_DRAM_TYPE,
	0,
	CHB_CS_CNT,
	CHB_COLUMN,
	(CHB_BANK/4)+1,
	CHB_BUS_WIDTH/16,
	CHB_EACH_DIE_BUS_WIDTH/16,
	CHB_EACH_DIE_6Gb_OR_12Gb,
	CHB_CS0_ROW,
	0,
	CHB_CS1_ROW,
	0
	}
};
pCH_INFO  pChInfo;
#else
extern DDR_TIMING_T ddr_timing;
extern CH_INFO  g_ChInfo[2];
extern pCH_INFO pChInfo;
#endif


/*
 * Description: According to channel information, and configuration
 *              row/bank/column relational register
 * Parameters :
 * Return     :
 * Note       : only use CS0, then cs=1
 *              use CS0&CS1, then cs=2
 *              8 bank, then bank = 3
 *              4 bank, then bank = 2
 */
unsigned int dram_cfg_rbc(unsigned int cs, unsigned int cs0_row,
	unsigned int cs1_row, unsigned int bank, unsigned int col,
	unsigned int bw);

/*
 * Description: check whether dram is in self-refresh mode
 * Parameters :
 * Return     : in self-refresh mode, return FALSE
 *              not in self-refresh mode, return TRUE
 * Note       :
 */
unsigned int dram_is_SelfRefresh(void);

/*
 * Description: dram initialize
 * Parameters :
 * Return     : initialize fail, return FALSE
 *              initialize success, return TRUE
 * Note       :
 */
unsigned int dram_init(unsigned int freq);

#endif
