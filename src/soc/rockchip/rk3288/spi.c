/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Samsung Electronics
 * Copyright 2013 Google Inc.
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

#include <console/console.h>
#include <arch/io.h>
#include <stdlib.h>
#include <assert.h>
#include <spi_flash.h>
#include <delay.h>


#include "memap.h"
#include "spi.h"
#include "reg.h"

#define EXYNOS_SPI_MAX_TRANSFER_BYTES (65535)


struct rockchip_spi_slave {
	struct spi_slave slave;
	struct rockchip_spi *regs;
	uint8_t bits_per_word;
	uint8_t			n_bytes;	/* current is a 1/2 bytes op */
	unsigned int 		speed_hz;
	unsigned int 		mode;
	unsigned int 		tmode;

	
	unsigned int fifo_size;
	uint8_t half_duplex;
	uint8_t frame_header;  /* header byte to detect in half-duplex mode. */
};

/* TODO(hungte) Move the SPI param list to per-board configuration, probably
 * Kconfig or mainboard.c */
static struct rockchip_spi_slave rockchip_spi_slaves[3] = {

	// SPI 0
	{
		.slave = { .bus = 0, },
		.regs = (void *)SPI0_BASE_ADDR,
	},
	// SPI 1
	{
		.slave = { .bus = 1, .rw = SPI_READ_FLAG, },
		.regs = (void *)SPI1_BASE_ADDR,
		.fifo_size = 64,
		.half_duplex = 0,
	},
	// SPI 2
	{
		.slave = { .bus = 2,
			   .rw = SPI_READ_FLAG | SPI_WRITE_FLAG, },
		.regs = (void *)SPI2_BASE_ADDR,
		.mode = 0,
		.bits_per_word = 8,
		.tmode = SPI_TMOD_TR,
		.speed_hz = 5000000,
		
		.fifo_size = 64,
		.half_duplex = 1,
		.frame_header = 0xec,
	},

};

static inline struct rockchip_spi_slave *to_rockchip_spi(struct spi_slave *slave)
{
	return container_of(slave, struct rockchip_spi_slave, slave);
}

void spi_init(void)
{
	
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	ASSERT(bus >= 0 && bus < 3);
	return &(rockchip_spi_slaves[bus].slave);
}

int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return bus > 0 && bus < 3;
}

void spi_cs_activate(struct spi_slave *slave)
{
	
	struct rockchip_spi *regs = to_rockchip_spi(slave)->regs;
	// TODO(hungte) Add some delay if too many transactions happen at once.
	setbits_le32(&regs->SPIM_SER, 1);
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	struct rockchip_spi *regs = to_rockchip_spi(slave)->regs;
	clrbits_le32(&regs->SPIM_SER, 1);
}

static inline void rockchip_spi_soft_reset(struct rockchip_spi *regs)
{

}

static inline void rockchip_spi_flush_fifo(struct rockchip_spi *regs)
{

}

static void rockchip_spi_request_bytes(struct rockchip_spi *regs, int count,
				     int width)
{
	writel(count-1, &regs->SPIM_CTRLR1);
}

static inline void rockchip_spi_enable_chip(struct rockchip_spi *regs, int enable)
{

	if(enable)
	{
		writel(1, &regs->SPIM_SPIENR);
	}
	else
	{
		writel(0, &regs->SPIM_SPIENR);
	}
}


static inline void rockchip_spi_set_clk(struct rockchip_spi *regs,  unsigned short div)
{
	writel(div, &regs->SPIM_BAUDR);
}


int spi_claim_bus(struct spi_slave *slave)
{
	struct rockchip_spi_slave *espi = to_rockchip_spi(slave);
	struct rockchip_spi *regs = espi->regs;
	
	unsigned int ctrlr0 = 0;
	unsigned short clk_div = 0;	
	unsigned char spi_dfs = 0, spi_tf = 0;
	
	g_cruReg->CRU_CLKSEL_CON[39] = 0x00970097;
	
	rockchip_spi_enable_chip(regs,0);
	
	switch (espi->bits_per_word) {
		case 8:
			espi->n_bytes = 1;
			spi_dfs = SPI_DFS_8BIT;
			spi_tf = SPI_HALF_WORLD_OFF;
			break;
		case 16:
			espi->n_bytes = 2;
			spi_dfs = SPI_DFS_16BIT;
			spi_tf = SPI_HALF_WORLD_ON;
			break;
		default:
			printk(BIOS_DEBUG,"MRST SPI: unsupported bits: %dbits\n", espi->bits_per_word);
	}
	/* Calculate clock divisor.  */
	clk_div = (200*1000*1000 / 2 / 4) / espi->speed_hz;
	clk_div = (clk_div + 1) & 0xfffe;
	rockchip_spi_set_clk(regs,clk_div);

	/* Operation Mode */
	ctrlr0 = (SPI_OMOD_MASTER << SPI_OMOD_OFFSET);

	/* Data Frame Size */
	ctrlr0 |= (spi_dfs & SPI_DFS_MASK) << SPI_DFS_OFFSET;

	/* set SPI mode 0..3 */
	if (espi->mode & SPI_CPOL)
		ctrlr0 = (SPI_SCOL_HIGH << SPI_SCOL_OFFSET);
	if (espi->mode & SPI_CPHA)
		ctrlr0 = (SPI_SCPH_TOGSTA << SPI_SCPH_OFFSET);

	/* Chip Select Mode */
	ctrlr0 |= (SPI_CSM_KEEP << SPI_CSM_OFFSET);

	/* SSN to Sclk_out delay */
	ctrlr0 |= (SPI_SSN_DELAY_ONE << SPI_SSN_DELAY_OFFSET);

	/* Serial Endian Mode */
	ctrlr0 |= (SPI_SEM_LITTLE << SPI_SEM_OFFSET);

	/* First Bit Mode */
	ctrlr0 |= (SPI_FBM_MSB << SPI_FBM_OFFSET);

	/* Byte and Halfword Transform */
	ctrlr0 |= ((spi_tf & SPI_HALF_WORLD_MASK) << SPI_HALF_WORLD_TX_OFFSET);

	/* Rxd Sample Delay */
	ctrlr0 |= (0 << SPI_RXDSD_OFFSET);

	/* Frame Format */
	ctrlr0 |= (SPI_FRF_SPI << SPI_FRF_OFFSET);

	/* Tx and Rx mode */
	ctrlr0 |= (espi->tmode & SPI_TMOD_MASK) << SPI_TMOD_OFFSET;
	writel(ctrlr0, &regs->SPIM_CTRLR0);

	return 0;
}

static int rockchip_spi_wait_till_not_busy(struct rockchip_spi *regs)
{
	unsigned int delay = 1000;

	while (delay--) {
		if (!(readl(&regs->SPIM_SR) & SR_BUSY)) {
			return 0;
		}

		//udelay(1);
	}

	printk(BIOS_DEBUG,"RK SPI: Status keeps busy for 1000us after a read/write!\n");
	return -1;
}

static int rockchip_spi_u8_reader(struct rockchip_spi *regs)
{
	int len = 0;
	while (!(readl(&regs->SPIM_SR) & SR_RF_EMPT)) {
		unsigned char rxdata = readl(&regs->SPIM_RXDR);
		//printk(BIOS_DEBUG,"rx data = %x\n",rxdata);
		rxdata ++;
		len ++;
	}

	rockchip_spi_wait_till_not_busy(regs);

	return len;
}

static int rockchip_spi_read(struct spi_slave *slave, void *dest, uint32_t len,
			   uint32_t off)
{
	struct rockchip_spi *regs = to_rockchip_spi(slave)->regs;
	int i;
	int count = 0;
	unsigned char * rxbuf = (unsigned char *)dest;
	uint32_t addr =off;

	rockchip_spi_enable_chip(regs,0);

	ASSERT(off < (1 << 24));
	rockchip_spi_request_bytes(regs, len,8);
	rockchip_spi_enable_chip(regs,1);
	printk(BIOS_DEBUG,"addr = %d\n",addr);
	spi_cs_activate(slave);
	
	writel(SF_READ_DATA_CMD, &regs->SPIM_TXDR);
	rockchip_spi_wait_till_not_busy(regs);
	writel((addr>>16)&0xff, &regs->SPIM_TXDR);
	rockchip_spi_wait_till_not_busy(regs);
	writel((addr>>8)&0xff, &regs->SPIM_TXDR);
	rockchip_spi_wait_till_not_busy(regs);
	writel((addr>>0)&0xff, &regs->SPIM_TXDR);
	rockchip_spi_wait_till_not_busy(regs);
	rockchip_spi_u8_reader(regs);
	
	while(count<len)
	{
		if (!(readl(&regs->SPIM_SR) & SR_TF_FULL))
		{
			writel(0xff, &regs->SPIM_TXDR);
			rockchip_spi_wait_till_not_busy(regs);
			while (!(readl(&regs->SPIM_SR) & SR_RF_EMPT)) {
				*rxbuf = readl(&regs->SPIM_RXDR);
				count ++;
				rxbuf ++;
				i++;
			}
		}
	}
	
	spi_cs_deactivate(slave);

	rockchip_spi_enable_chip(regs,0);

	return count;
}

void spi_release_bus(struct spi_slave *slave)
{
	struct rockchip_spi *regs = to_rockchip_spi(slave)->regs;
	rockchip_spi_flush_fifo(regs);
}


// SPI as CBFS media.
struct rockchip_spi_media {
	struct spi_slave *slave;
	struct cbfs_simple_buffer buffer;
};

static int rockchip_spi_cbfs_open(struct cbfs_media *media)
{
	struct rockchip_spi_media *spi = (struct rockchip_spi_media*)media->context;
	return spi_claim_bus(spi->slave);
}

static int rockchip_spi_cbfs_close(struct cbfs_media *media)
{
	struct rockchip_spi_media *spi = (struct rockchip_spi_media*)media->context;
	spi_release_bus(spi->slave);
	return 0;
}

static size_t rockchip_spi_cbfs_read(struct cbfs_media *media, void *dest,
				   size_t offset, size_t count)
{
	struct rockchip_spi_media *spi = (struct rockchip_spi_media*)media->context;
	int bytes;
	bytes = rockchip_spi_read(spi->slave, dest, count, offset);
	return bytes;
}

static void *rockchip_spi_cbfs_map(struct cbfs_media *media, size_t offset,
				 size_t count)
{
	struct rockchip_spi_media *spi = (struct rockchip_spi_media*)media->context;
	printk(BIOS_DEBUG,"rockchip_spi_cbfs_map,offset=%d,count=%d\n",offset,count);

	if (count % 4)
		count += 4 - (count % 4);
	return cbfs_simple_buffer_map(&spi->buffer, media, offset, count);
}

static void *rockchip_spi_cbfs_unmap(struct cbfs_media *media,
				   const void *address)
{
	struct rockchip_spi_media *spi = (struct rockchip_spi_media*)media->context;
	printk(BIOS_DEBUG,"rockchip_spi_cbfs_unmap\n");
	return cbfs_simple_buffer_unmap(&spi->buffer, address);
}

int initialize_rockchip_spi_cbfs_media(struct cbfs_media *media,
				     void *buffer_address,
				     size_t buffer_size)
{
	static struct rockchip_spi_media context;
	static struct rockchip_spi_slave *eslave = &rockchip_spi_slaves[2];
	printk(BIOS_DEBUG,"initialize_rockchip_spi_cbfs_media\n");
	context.slave = &eslave->slave;
	context.buffer.allocated = context.buffer.last_allocate = 0;
	context.buffer.buffer = buffer_address;
	context.buffer.size = buffer_size;
	media->context = (void*)&context;
	media->open = rockchip_spi_cbfs_open;
	media->close = rockchip_spi_cbfs_close;
	media->read = rockchip_spi_cbfs_read;
	media->map = rockchip_spi_cbfs_map;
	media->unmap = rockchip_spi_cbfs_unmap;

	return 0;
}
