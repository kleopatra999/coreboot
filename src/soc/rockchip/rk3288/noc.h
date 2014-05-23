#ifndef CPU_ROCKCHIP_RK3288_NOC_H
#define CPU_ROCKCHIP_RK3288_NOC_H

typedef union NOC_TIMING_Tag {
	unsigned int d32;
	struct {
		unsigned ActToAct:6;
		unsigned RdToMiss:6;
		unsigned WrToMiss:6;
		unsigned BurstLen:3;
		unsigned RdToWr:5;
		unsigned WrToRd:5;
		unsigned BwRatio:1;
	} b;
} NOC_TIMING_T;

typedef union NOC_ACTIVATE_Tag {
	unsigned int d32;
	struct {
		unsigned Rrd:4;
		unsigned Faw:6;
		unsigned Fawbank:1;
		unsigned reserved:21;
	} b;
} NOC_ACTIVATE_T;

typedef struct MSCH_REG_Tag {
	unsigned int coreid;
	unsigned int revisionid;
	unsigned int ddrconf;
	NOC_TIMING_T ddrtiming;
	unsigned int ddrmode;
	unsigned int readlatency;
	int reserved1[(0x38-0x18)/4];
	NOC_ACTIVATE_T activate;
	unsigned int devtodev;
} MSCH_REG, *pMSCH_REG;

typedef struct QOS_REG_Tag {
	int reserved1[2];
	unsigned int priority;
	unsigned int mode;
	unsigned int bandwidth;
	unsigned int saturation;
	unsigned int extcontrol;
} QOS_REG, *pQOS_REG;

/* use this row/bank/column information to find register value index  */
extern unsigned short int  ddr_cfg_2_rbc[16];
#endif
