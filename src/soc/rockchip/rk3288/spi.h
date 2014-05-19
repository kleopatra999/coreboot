/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef CPU_ROCKCHIP_RK3288_SPI_H
#define CPU_ROCKCHIP_RK3288_SPI_H

/* This driver serves as a CBFS media source. */
#include <cbfs.h>


//SPI MASTER Registers
struct rockchip_spi
{
    unsigned int SPIM_CTRLR0;
    unsigned int SPIM_CTRLR1;
    unsigned int SPIM_SPIENR;
    unsigned int SPIM_SER;
    unsigned int SPIM_BAUDR; //0x10
    unsigned int SPIM_TXFTLR;
    unsigned int SPIM_RXFTLR;
    unsigned int SPIM_TXFLR;
    unsigned int SPIM_RXFLR; //0x20
    unsigned int SPIM_SR;
    unsigned int SPIM_IPR; // 29 add
    unsigned int SPIM_IMR;
    unsigned int SPIM_ISR; //0x30
    unsigned int SPIM_RISR;
    unsigned int SPIM_ICR;
    unsigned int SPIM_DMACR;
    unsigned int SPIM_DMATDLR; //0x40
    unsigned int SPIM_DMARDLR;
    unsigned int reserved[(0x400-0x48)/4];
    unsigned int SPIM_TXDR[0x100];
    unsigned int SPIM_RXDR[0x100];
};

#define SF_READ_DATA_CMD	0x3

/* --------Bit fields in CTRLR0--------begin */

#define SPI_DFS_OFFSET			0                  /* Data Frame Size */
#define SPI_DFS_MASK			0x3
#define SPI_DFS_4BIT			0x00
#define SPI_DFS_8BIT			0x01
#define SPI_DFS_16BIT			0x02
#define SPI_DFS_RESV			0x03

#define SPI_CFS_OFFSET			2                  /* Control Frame Size */
#define SPI_CFS_MASK			0xF

#define SPI_SCPH_OFFSET			6                  /* Serial Clock Phase */
#define SPI_SCPH_MASK			0x1
#define SPI_SCPH_TOGMID			0                  /* Serial clock toggles in middle of first data bit */
#define SPI_SCPH_TOGSTA			1                  /* Serial clock toggles at start of first data bit */

#define SPI_SCOL_OFFSET			7                  /* Serial Clock Polarity */
#define SPI_SCOL_MASK			0x1
#define SPI_SCOL_LOW			0                  /* Inactive state of clock serial clock is low */
#define SPI_SCOL_HIGH			1                  /* Inactive state of clock serial clock is high */

#define SPI_CSM_OFFSET			8                  /* Chip Select Mode */
#define SPI_CSM_MASK			0x3
#define SPI_CSM_KEEP			0x00               /* ss_n keep low after every frame data is transferred */
#define SPI_CSM_HALF			0x01               /* ss_n be high for half sclk_out cycles after every frame data is transferred */
#define SPI_CSM_ONE			0x02               /* ss_n be high for one sclk_out cycle after every frame data is transferred */
#define SPI_CSM_RESV			0x03

#define SPI_SSN_DELAY_OFFSET		10                 /* SSN to Sclk_out delay */
#define SPI_SSN_DELAY_MASK		0x1
#define SPI_SSN_DELAY_HALF		0x00               /* the peroid between ss_n active and sclk_out active is half sclk_out cycles */
#define SPI_SSN_DELAY_ONE		0x01               /* the peroid between ss_n active and sclk_out active is one sclk_out cycle */

#define SPI_SEM_OFFSET			11                 /* Serial Endian Mode */
#define SPI_SEM_MASK			0x1
#define SPI_SEM_LITTLE			0x00               /* little endian */
#define SPI_SEM_BIG			0x01               /* big endian */

#define SPI_FBM_OFFSET			12                 /* First Bit Mode */
#define SPI_FBM_MASK			0x1
#define SPI_FBM_MSB			0x00               /* first bit in MSB */
#define SPI_FBM_LSB			0x01               /* first bit in LSB */

#define SPI_HALF_WORLD_TX_OFFSET	13                 /* Byte and Halfword Transform */
#define SPI_HALF_WORLD_MASK		0x1
#define SPI_HALF_WORLD_ON		0x00               /* apb 16bit write/read, spi 8bit write/read */
#define SPI_HALF_WORLD_OFF		0x01               /* apb 8bit write/read, spi 8bit write/read */

#define SPI_RXDSD_OFFSET		14                 /* Rxd Sample Delay */
#define SPI_RXDSD_MASK			0x3

#define SPI_FRF_OFFSET			16                 /* Frame Format */
#define SPI_FRF_MASK			0x3
#define SPI_FRF_SPI			0x00               /* motorola spi */
#define SPI_FRF_SSP			0x01               /* Texas Instruments SSP*/
#define SPI_FRF_MICROWIRE		0x02               /*  National Semiconductors Microwire */
#define SPI_FRF_RESV			0x03

#define SPI_TMOD_OFFSET			18                 /* Transfer Mode */
#define SPI_TMOD_MASK			0x3
#define	SPI_TMOD_TR			0x00		   /* xmit & recv */
#define SPI_TMOD_TO			0x01		   /* xmit only */
#define SPI_TMOD_RO			0x02		   /* recv only */
#define SPI_TMOD_RESV			0x03

#define SPI_OMOD_OFFSET			20                 /* Operation Mode */
#define SPI_OMOD_MASK			0x1
#define	SPI_OMOD_MASTER			0x00		   /* Master Mode */
#define SPI_OMOD_SLAVE			0x01		   /* Slave Mode */

/* --------Bit fields in CTRLR0--------end */


/* Bit fields in SR, 7 bits */
#define SR_MASK				0x7f		/* cover 7 bits */
#define SR_BUSY				(1 << 0)
#define SR_TF_FULL			(1 << 1)
#define SR_TF_EMPT			(1 << 2)
#define SR_RF_EMPT			(1 << 3)
#define SR_RF_FULL			(1 << 4)

/* Bit fields in ISR, IMR, RISR, 7 bits */
#define SPI_INT_TXEI			(1 << 0)
#define SPI_INT_TXOI			(1 << 1)
#define SPI_INT_RXUI			(1 << 2)
#define SPI_INT_RXOI			(1 << 3)
#define SPI_INT_RXFI			(1 << 4)

/* Bit fields in DMACR */
#define SPI_DMACR_TX_ENABLE		(1 << 1)
#define SPI_DMACR_RX_ENABLE		(1 << 0)

/* Bit fields in ICR */
#define SPI_CLEAR_INT_ALL		(1<< 0)
#define SPI_CLEAR_INT_RXUI		(1 << 1)
#define SPI_CLEAR_INT_RXOI		(1 << 2)
#define SPI_CLEAR_INT_TXOI		(1 << 3)

/* Serve as CBFS media source */
int initialize_rockchip_spi_cbfs_media(struct cbfs_media *media,
				     void *buffer_address,
				     size_t buffer_size);
#endif
