#ifndef CPU_ROCKCHIP_RK3288_DRAMPHY_H
#define CPU_ROCKCHIP_RK3288_DRAMPHY_H

/* PIR */
#define INIT                 (1<<0)
#define DLLSRST              (1<<1)
#define DLLLOCK              (1<<2)
#define ZCAL                 (1<<3)
#define ITMSRST              (1<<4)
#define DRAMRST              (1<<5)
#define DRAMINIT             (1<<6)
#define QSTRN                (1<<7)
#define RVTRN                (1<<8)
#define ICPC                 (1<<16)
#define DLLBYP               (1<<17)
#define CTLDINIT             (1<<18)
#define CLRSR                (1<<28)
#define LOCKBYP              (1<<29)
#define ZCALBYP              (1<<30)
#define INITBYP              (1u<<31)

/* PGCR */
#define DFTLMT(n)            ((n)<<3)
#define DFTCMP(n)            ((n)<<2)
#define DQSCFG(n)            ((n)<<1)
#define ITMDMD(n)            ((n)<<0)
#define RANKEN(n)            ((n)<<18)

/* PGSR */
#define IDONE                (1<<0)
#define DLDONE               (1<<1)
#define ZCDONE               (1<<2)
#define DIDONE               (1<<3)
#define DTDONE               (1<<4)
#define DTERR                (1<<5)
#define DTIERR               (1<<6)
#define DFTERR               (1<<7)
#define RVERR                (1<<8)
#define RVEIRR               (1<<9)
#define TQ                   (1u<<31)

/* PTR0 */
#define tITMSRST(n)          ((n)<<18)
#define tDLLLOCK(n)          ((n)<<6)
#define tDLLSRST(n)          ((n)<<0)

/* PTR1 */
#define tDINIT1(n)           ((n)<<19)
#define tDINIT0(n)           ((n)<<0)

/* PTR2 */
#define tDINIT3(n)           ((n)<<17)
#define tDINIT2(n)           ((n)<<0)

/* DXCCR */
#define RVSEL(n)             ((n)<<15)

/* DSGCR */
#define DQSGE(n)             ((n)<<8)
#define DQSGX(n)             ((n)<<5)
#define NOBUB                (1<<11)
#define FXDLAT               (1<<12)

/* DCUAR */
#define CWADDR(n)            (n)
#define CSADDR(n)            ((n)<<4)
#define CMD_cache            (0<<8)
#define EXP_data_cache       (1<<8)
#define RD_data_cache        (2<<8)
#define INCA                 (1<<10)
#define Write_access         (0<<11)
#define Read_access          (1<<11)

/* DCUDR */
#define DCUDR_MASK(n)        (n)
#define DCUDR_ADDR(n)        ((n)<<8)
#define DCUDR_BANK(n)        ((n)<<24)
#define DCUDR_RANK(n)        ((n)<<27)
#define DCUDR_CMD(n)         ((n)<<28)

#define DCUDR_TAG(n)         (n)
#define DCUDR_DTP(n)         ((n)<<2)
#define DCUDR_RTP(n)         ((n)<<7)

/* DCURR */
#define DINST_Run            (1)
#define SADDR(n)             ((n)<<4)
#define EADDR(n)             ((n)<<8)
#define NFAIL(n)             ((n)<<12)
#define SONF                 (1<<20)
#define SCOF                 (1<<21)
#define RCEN                 (1<<22)
#define XCEN                 (1<<23)

/* DCULR */
#define LSADDR(n)            (n)
#define LEADDR(n)            ((n)<<4)
#define LCNT(n)              ((n)<<8)
#define LINF                 (1<<16)
#define IDA                  (1<<17)
#define XLEADDR(n)           ((n)<<28)

/* DCUTPR */
#define tDCUT0(n)            (n)
#define tDCUT1(n)            ((n)<<8)
#define tDCUT2(n)            ((n)<<16)
#define tDCUT3(n)            ((n)<<24)

/* DCUSR0 */
#define RDONE                 (1)
#define CFAIL                 (1<<1)
#define CFULL                 (1<<2)

/* DCUSR1 */
#define RDCNT(n)              ((n) & 0xFFFF)
#define FLCNT(n)              (((n)>>16) & 0xFF)
#define LPCNT(n)              (((n)>>24) & 0xFF)


typedef union DCR_Tag {
	unsigned int d32;
	struct {
		unsigned DDRMD:3;
		unsigned DDR8BNK:1;
		unsigned PDQ:3;
		unsigned MPRDQ:1;
		unsigned DDRTYPE:2;
		unsigned reserved10_26:17;
		unsigned NOSRA:1;
		unsigned DDR2T:1;
		unsigned UDIMM:1;
		unsigned RDIMM:1;
		unsigned TPD:1;
	} b;
} DCR_T;


typedef struct DATX8_REG_Tag {
	/* DATX8 General Configuration Register */
	volatile unsigned int DXGCR;
	/* DATX8 General Status Register */
	volatile unsigned int DXGSR[2];
	/* DATX8 DLL Control Register */
	volatile unsigned int DXDLLCR;
	/* DATX8 DQ Timing Register */
	volatile unsigned int DXDQTR;
	/* DATX8 DQS Timing Register */
	volatile unsigned int DXDQSTR;
	volatile unsigned int reserved[0x80-0x76];
} DATX8_REG_T;

/* DDR PHY register struct */
typedef volatile struct DDRPHY_REG_Tag {
	/* Revision Identification Register */
	volatile unsigned int RIDR;
	/* PHY Initialization Register */
	volatile unsigned int PIR;
	/* PHY General Configuration Register */
	volatile unsigned int PGCR;
	/* PHY General Status Register */
	volatile unsigned int PGSR;
	/* DLL General Control Register */
	volatile unsigned int DLLGCR;
	/* AC DLL Control Register */
	volatile unsigned int ACDLLCR;
	/* PHY Timing Registers 0-2 */
	volatile unsigned int PTR[3];
	/* AC I/O Configuration Register */
	volatile unsigned int ACIOCR;
	/* DATX8 Common Configuration Register */
	volatile unsigned int DXCCR;
	/* DDR System General Configuration Register */
	volatile unsigned int DSGCR;
	/* DRAM Configuration Register */
	DCR_T DCR;
	/* DRAM Timing Parameters Register 0-2 */
	volatile unsigned int DTPR[3];
	/* Mode Register 0-3 */
	volatile unsigned int MR[4];
	/* ODT Configuration Register */
	volatile unsigned int ODTCR;
	/* Data Training Address Register */
	volatile unsigned int DTAR;
	/* Data Training Data Register 0-1 */
	volatile unsigned int DTDR[2];

	unsigned int reserved1[0x30-0x18];
	/* DCU Address Register */
	unsigned int DCUAR;
	/* DCU Data Register */
	unsigned int DCUDR;
	/* DCU Run Register */
	unsigned int DCURR;
	/* DCU Loop Register */
	unsigned int DCULR;
	/* DCU General Configuration Register */
	unsigned int DCUGCR;
	/* DCU Timing Parameters Register */
	unsigned int DCUTPR;
	/* DCU Status Register 0-1 */
	unsigned int DCUSR[2];
	unsigned int reserved2[0x40-0x38];
	unsigned int BIST[0x51-0x40];
	unsigned int reserved3[0x60-0x51];

	/* ZQ 0 Impedance Control Register 0-1 */
	volatile unsigned int ZQ0CR[2];
	/* ZQ 0 Impedance Status Register 0-1 */
	volatile unsigned int ZQ0SR[2];
	/* ZQ 1 Impedance Control Register 0-1 */
	volatile unsigned int ZQ1CR[2];
	/* ZQ 1 Impedance Status Register 0-1 */
	volatile unsigned int ZQ1SR[2];
	/* ZQ 2 Impedance Control Register 0-1 */
	volatile unsigned int ZQ2CR[2];
	/* ZQ 2 Impedance Status Register 0-1 */
	volatile unsigned int ZQ2SR[2];
	/* ZQ 3 Impedance Control Register 0-1 */
	volatile unsigned int ZQ3CR[2];
	/* ZQ 3 Impedance Status Register 0-1 */
	volatile unsigned int ZQ3SR[2];
	/* DATX8 Register */
	DATX8_REG_T     DATX8[9];
} DDRPHY_REG_T, *pDDRPHY_REG_T;

#endif
