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

typedef volatile struct MSCH_REG_Tag {
	volatile unsigned int coreid;
	volatile unsigned int revisionid;
	volatile unsigned int ddrconf;
	volatile NOC_TIMING_T ddrtiming;
	volatile unsigned int ddrmode;
	volatile unsigned int readlatency;
	unsigned int reserved1[(0x38-0x18)/4];
	volatile NOC_ACTIVATE_T activate;
	volatile unsigned int devtodev;
} MSCH_REG, *pMSCH_REG;

typedef volatile struct QOS_REG_Tag {
	unsigned int reserved1[2];
	volatile unsigned int priority;
	volatile unsigned int mode;
	volatile unsigned int bandwidth;
	volatile unsigned int saturation;
	volatile unsigned int extcontrol;
} QOS_REG, *pQOS_REG;

/* use this row/bank/column information to find register value index  */
extern unsigned short int  ddr_cfg_2_rbc[16];
#endif
