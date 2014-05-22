#ifndef CPU_ROCKCHIP_RK3288_DRAMCTL_H
#define CPU_ROCKCHIP_RK3288_DRAMCTL_H


/* SCTL */
#define INIT_STATE                     (0)
#define CFG_STATE                      (1)
#define GO_STATE                       (2)
#define SLEEP_STATE                    (3)
#define WAKEUP_STATE                   (4)

/* STAT */
#define Init_mem                       (0)
#define Config                         (1)
#define Config_req                     (2)
#define Access                         (3)
#define Access_req                     (4)
#define Low_power                      (5)
#define Low_power_entry_req            (6)
#define Low_power_exit_req             (7)

/* MCFG */
#define mddr_lpddr2_clk_stop_idle(n)   ((n)<<24)
#define pd_idle(n)                     ((n)<<8)
#define mddr_en                        (2<<22)
#define lpddr2_en                      (3<<22)
#define ddr2_en                        (0<<5)
#define ddr3_en                        (1<<5)
#define lpddr2_s2                      (0<<6)
#define lpddr2_s4                      (1<<6)
#define mddr_lpddr2_bl_2               (0<<20)
#define mddr_lpddr2_bl_4               (1<<20)
#define mddr_lpddr2_bl_8               (2<<20)
#define mddr_lpddr2_bl_16              (3<<20)
#define ddr2_ddr3_bl_4                 (0)
#define ddr2_ddr3_bl_8                 (1)
#define tfaw_cfg(n)                    (((n)-4)<<18)
#define pd_exit_slow                   (0<<17)
#define pd_exit_fast                   (1<<17)
#define pd_type(n)                     ((n)<<16)
#define two_t_en(n)                    ((n)<<3)
#define bl8int_en(n)                   ((n)<<2)
#define cke_or_en(n)                   ((n)<<1)

/* POWCTL */
#define power_up_start                 (1<<0)

/* POWSTAT */
#define power_up_done                  (1<<0)

/* DFISTSTAT0 */
#define dfi_init_complete              (1<<0)

/* CMDTSTAT*/
#define cmd_tstat                      (1<<0)

/* CMDTSTATEN */
#define cmd_tstat_en                   (1<<1)

/* MCMD */
#define Deselect_cmd                   (0)
#define PREA_cmd                       (1)
#define REF_cmd                        (2)
#define MRS_cmd                        (3)
#define ZQCS_cmd                       (4)
#define ZQCL_cmd                       (5)
#define RSTL_cmd                       (6)
#define MRR_cmd                        (8)
#define DPDE_cmd                       (9)

#define lpddr2_op(n)                   (((n)&0xff)<<12)
#define lpddr2_ma(n)                   (((n)&0xff)<<4)

#define bank_addr(n)                   (((n)&0x7)<<17)
#define cmd_addr(n)                    (((n)&0x1fff)<<4)

#define start_cmd                      (1u<<31)

typedef union STAT_Tag {
	unsigned int d32;
	struct {
		unsigned ctl_stat:3;
		unsigned reserved3:1;
		unsigned lp_trig:3;
		unsigned reserved7_31:25;
	} b;
} STAT_T;

typedef union SCFG_Tag {
	unsigned int d32;
	struct {
		unsigned hw_low_power_en:1;
		unsigned reserved1_5:5;
		unsigned nfifo_nif1_dis:1;
		unsigned reserved7:1;
		unsigned bbflags_timing:4;
		unsigned reserved12_31:20;
	} b;
} SCFG_T;

/* DDR Controller register struct */
typedef struct DDR_REG_Tag {
	/* Operational State, Control, and Status Registers */
	/* State Configuration Register */
	SCFG_T SCFG;
	/* State Control Register */
	unsigned int SCTL;
	/* State Status Register */
	STAT_T STAT;
	/* Interrupt Status Register */
	unsigned int INTRSTAT;
	unsigned int reserved0[(0x40-0x10)/4];
	/* Initailization Control and Status Registers */
	/* Memory Command Register */
	unsigned int MCMD;
	/* Power Up Control Registers */
	unsigned int POWCTL;
	/* Power Up Status Register */
	unsigned int POWSTAT;
	/* Command Timing Status Register */
	unsigned int CMDTSTAT;
	/* Command Timing Status Enable Register */
	unsigned int CMDTSTATEN;
	unsigned int reserved1[(0x60-0x54)/4];
	/* MRR Configuration 0 Register */
	unsigned int MRRCFG0;
	/* MRR Status 0 Register */
	unsigned int MRRSTAT0;
	/* MRR Status 1 Register */
	unsigned int MRRSTAT1;
	unsigned int reserved2[(0x7c-0x6c)/4];
	/* Memory Control and Status Registers */
	/* Memory Configuration 1 Register */
	unsigned int MCFG1;
	/* Memory Configuration Register */
	unsigned int MCFG;
	/* Partially Populated Memories Configuration Register */
	unsigned int PPCFG;
	/* Memory Status Register */
	unsigned int MSTAT;
	/* LPDDR2 ZQ Configuration Register */
	unsigned int LPDDR2ZQCFG;
	unsigned int reserved3;
	/* DTU Control and Status Registers */
	/* DTU Status Register */
	unsigned int DTUPDES;
	/* DTU Number of Random Addresses Created Register */
	unsigned int DTUNA;
	/* DTU Number of Errors Register */
	unsigned int DTUNE;
	/* DTU Parallel Read 0 */
	unsigned int DTUPRD0;
	/* DTU Parallel Read 1 */
	unsigned int DTUPRD1;
	/* DTU Parallel Read 2 */
	unsigned int DTUPRD2;
	/* DTU Parallel Read 3 */
	unsigned int DTUPRD3;
	/* DTU Address Width */
	unsigned int DTUAWDT;
	unsigned int reserved4[(0xc0-0xb4)/4];
	/* Memory Timing Registers */
	/* Toggle Counter 1U Register */
	unsigned int TOGCNT1U;
	/* t_init Timing Register */
	unsigned int TINIT;
	/* Reset High Time Register */
	unsigned int TRSTH;
	/* Toggle Counter 100N Register */
	unsigned int TOGCNT100N;
	/* t_refi Timing Register */
	unsigned int TREFI;
	/* t_mrd Timing Register */
	unsigned int TMRD;
	/* t_rfc Timing Register */
	unsigned int TRFC;
	/* t_rp Timing Register */
	unsigned int TRP;
	/* t_rtw Timing Register */
	unsigned int TRTW;
	/* AL Latency Register */
	unsigned int TAL;
	/* CL Timing Register */
	unsigned int TCL;
	/* CWL Register */
	unsigned int TCWL;
	/* t_ras Timing Register */
	unsigned int TRAS;
	/* t_rc Timing Register */
	unsigned int TRC;
	/* t_rcd Timing Register */
	unsigned int TRCD;
	/* t_rrd Timing Register */
	unsigned int TRRD;
	/* t_rtp Timing Register */
	unsigned int TRTP;
	/* t_wr Timing Register */
	unsigned int TWR;
	/* t_wtr Timing Register */
	unsigned int TWTR;
	/* t_exsr Timing Register */
	unsigned int TEXSR;
	/* t_xp Timing Register */
	unsigned int TXP;
	/* t_xpdll Timing Register */
	unsigned int TXPDLL;
	/* t_zqcs Timing Register */
	unsigned int TZQCS;
	/* t_zqcsi Timing Register */
	unsigned int TZQCSI;
	/* t_dqs Timing Register */
	unsigned int TDQS;
	/* t_cksre Timing Register */
	unsigned int TCKSRE;
	/* t_cksrx Timing Register */
	unsigned int TCKSRX;
	/* t_cke Timing Register */
	unsigned int TCKE;
	/* t_mod Timing Register */
	unsigned int TMOD;
	/* Reset Low Timing Register */
	unsigned int TRSTL;
	/* t_zqcl Timing Register */
	unsigned int TZQCL;
	/* t_mrr Timing Register */
	unsigned int TMRR;
	/* t_ckesr Timing Register */
	unsigned int TCKESR;
	/* t_dpd Timing Register */
	unsigned int TDPD;
	unsigned int reserved5[(0x180-0x148)/4];
	/* ECC Configuration, Control, and Status Registers */
	/* ECC Configuration Register */
	unsigned int ECCCFG;
	/* ECC Test Register */
	unsigned int ECCTST;
	/* ECC Clear Register */
	unsigned int ECCCLR;
	/* ECC Log Register */
	unsigned int ECCLOG;
	unsigned int reserved6[(0x200-0x190)/4];
	/* DTU Control and Status Registers */
	/* DTU Write Address Control Register */
	unsigned int DTUWACTL;
	/* DTU Read Address Control Register */
	unsigned int DTURACTL;
	/* DTU Configuration Control Register */
	unsigned int DTUCFG;
	/* DTU Execute Control Register */
	unsigned int DTUECTL;
	/* DTU Write Data 0 */
	unsigned int DTUWD0;
	/* DTU Write Data 1 */
	unsigned int DTUWD1;
	/* DTU Write Data 2 */
	unsigned int DTUWD2;
	/* DTU Write Data 3 */
	unsigned int DTUWD3;
	/* DTU Write Data Mask */
	unsigned int DTUWDM;
	/* DTU Read Data 0 */
	unsigned int DTURD0;
	/* DTU Read Data 1 */
	unsigned int DTURD1;
	/* DTU Read Data 2 */
	unsigned int DTURD2;
	/* DTU Read Data 3 */
	unsigned int DTURD3;
	/* DTU LFSR Seed for Write Data Generation */
	unsigned int DTULFSRWD;
	/* DTU LFSR Seed for Read Data Generation */
	unsigned int DTULFSRRD;
	/* DTU Error Address FIFO */
	unsigned int DTUEAF;
	/* DFI Control Registers */
	/* DFI tctrl_delay Register */
	unsigned int DFITCTRLDELAY;
	/* DFI ODT Configuration Register */
	unsigned int DFIODTCFG;
	/* DFI ODT Configuration 1 Register */
	unsigned int DFIODTCFG1;
	/* DFI ODT Rank Mapping Register */
	unsigned int DFIODTRANKMAP;
	/* DFI Write Data Registers */
	/* DFI tphy_wrdata Register */
	unsigned int DFITPHYWRDATA;
	/* DFI tphy_wrlat Register */
	unsigned int DFITPHYWRLAT;
	unsigned int reserved7[(0x260-0x258)/4];
	/* DFI trddata_en Register */
	unsigned int DFITRDDATAEN;
	/* DFI tphy_rddata Register */
	unsigned int DFITPHYRDLAT;
	unsigned int reserved8[(0x270-0x268)/4];
	/* DFI Update Registers */
	/* DFI tphyupd_type0 Register */
	unsigned int DFITPHYUPDTYPE0;
	/* DFI tphyupd_type1 Register */
	unsigned int DFITPHYUPDTYPE1;
	/* DFI tphyupd_type2 Register */
	unsigned int DFITPHYUPDTYPE2;
	/* DFI tphyupd_type3 Register */
	unsigned int DFITPHYUPDTYPE3;
	/* DFI tctrlupd_min Register */
	unsigned int DFITCTRLUPDMIN;
	/* DFI tctrlupd_max Register */
	unsigned int DFITCTRLUPDMAX;
	/* DFI tctrlupd_dly Register */
	unsigned int DFITCTRLUPDDLY;
	unsigned int reserved9;
	/* DFI Update Configuration Register */
	unsigned int DFIUPDCFG;
	/* DFI Masked Refresh Interval Register */
	unsigned int DFITREFMSKI;
	/* DFI tctrlupd_interval Register */
	unsigned int DFITCTRLUPDI;
	unsigned int reserved10[(0x2ac-0x29c)/4];
	/* DFI Training Configuration 0 Register */
	unsigned int DFITRCFG0;
	/* DFI Training Status 0 Register */
	unsigned int DFITRSTAT0;
	/* DFI Training dfi_wrlvl_en Register */
	unsigned int DFITRWRLVLEN;
	/* DFI Training dfi_rdlvl_en Register */
	unsigned int DFITRRDLVLEN;
	/* DFI Training dfi_rdlvl_gate_en Register */
	unsigned int DFITRRDLVLGATEEN;
	/* DFI Status Registers */
	/* DFI Status Status 0 Register */
	unsigned int DFISTSTAT0;
	/* DFI Status Configuration 0 Register */
	unsigned int DFISTCFG0;
	/* DFI Status configuration 1 Register */
	unsigned int DFISTCFG1;
	unsigned int reserved11;
	/* DFI tdram_clk_enalbe Register */
	unsigned int DFITDRAMCLKEN;
	/* DFI tdram_clk_disalbe Register */
	unsigned int DFITDRAMCLKDIS;
	/* DFI Status configuration 2 Register */
	unsigned int DFISTCFG2;
	/* DFI Status Parity Clear Register */
	unsigned int DFISTPARCLR;
	/* DFI Status Parity Log Register */
	unsigned int DFISTPARLOG;
	unsigned int reserved12[(0x2f0-0x2e4)/4];
	/* DFI Low Power Registers */
	/* DFI Low Power Configuration 0 Register */
	unsigned int DFILPCFG0;
	unsigned int reserved13[(0x300-0x2f4)/4];
	/* DFI Training 2 Registers */
	/* DFI Training dif_wrlvl_resp Status 0 Register */
	unsigned int DFITRWRLVLRESP0;
	/* DFI Training dif_wrlvl_resp Status 1 Register */
	unsigned int DFITRWRLVLRESP1;
	/* DFI Training dif_wrlvl_resp Status 2 Register */
	unsigned int DFITRWRLVLRESP2;
	/* DFI Training dif_rdlvl_resp Status 0 Register */
	unsigned int DFITRRDLVLRESP0;
	/* DFI Training dif_rdlvl_resp Status 1 Register */
	unsigned int DFITRRDLVLRESP1;
	/* DFI Training dif_rdlvl_resp Status 2 Register */
	unsigned int DFITRRDLVLRESP2;
	/* DFI Training dif_wrlvl_delay Configuration 0 Register */
	unsigned int DFITRWRLVLDELAY0;
	/* DFI Training dif_wrlvl_delay Configuration 1 Register */
	unsigned int DFITRWRLVLDELAY1;
	/* DFI Training dif_wrlvl_delay Configuration 2 Register */
	unsigned int DFITRWRLVLDELAY2;
	/* DFI Training dif_rdlvl_delay Configuration 0 Register */
	unsigned int DFITRRDLVLDELAY0;
	/* DFI Training dif_rdlvl_delay Configuration 1 Register */
	unsigned int DFITRRDLVLDELAY1;
	/* DFI Training dif_rdlvl_delay Configuration 2 Register */
	unsigned int DFITRRDLVLDELAY2;
	/* DFI Training dif_rdlvl_gate_delay Configuration 0 Register */
	unsigned int DFITRRDLVLGATEDELAY0;
	/* DFI Training dif_rdlvl_gate_delay Configuration 1 Register */
	unsigned int DFITRRDLVLGATEDELAY1;
	/* DFI Training dif_rdlvl_gate_delay Configuration 2 Register */
	unsigned int DFITRRDLVLGATEDELAY2;
	/* DFI Training Command Register */
	unsigned int DFITRCMD;
	unsigned int reserved14[(0x3f8-0x340)/4];
	/* IP Status Registers */
	/* IP Version Register */
	unsigned int IPVR;
	/* IP Type Register */
	unsigned int IPTR;
} DDR_REG_T, *pDDR_REG_T;
#endif
