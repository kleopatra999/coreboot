#include <console/console.h>
#include <arch/io.h>
#include <stdlib.h>
#include <assert.h>
#include <spi_flash.h>
#include <delay.h>
#include "memap.h"
#include "spi.h"
#include "reg.h"
#include "clock.h"

struct rockchip_spi_slave {
	struct spi_slave slave;
	struct rockchip_spi *regs;
	uint8_t bits_per_word;
	uint8_t	n_bytes;	/* current is a 1/2 bytes op */
	unsigned int	speed_hz;
	unsigned int	mode;
	unsigned int	tmode;
	unsigned int fifo_size;
	uint8_t half_duplex;
	uint8_t frame_header;  /* header byte to detect in half-duplex mode. */
};

/* TODO(hungte) Move the SPI param list to per-board configuration, probably
 * Kconfig or mainboard.c */
static struct rockchip_spi_slave rockchip_spi_slaves[3] = {
	{
		.slave = {
			.bus = 0,
			.rw = SPI_READ_FLAG | SPI_WRITE_FLAG,
			},
		.regs = (void *)SPI0_BASE_ADDR,
		.mode = 0,
		.bits_per_word = 8,
		.tmode = SPI_TMOD_TR,
		.speed_hz = 12000000,
		.fifo_size = 32,
		.half_duplex = 1,
		.frame_header = 0xec,
	},
	{
		.slave = {.bus = 1, .rw = SPI_READ_FLAG,},
		.regs = (void *)SPI1_BASE_ADDR,
		.mode = 0,
		.bits_per_word = 8,
		.tmode = SPI_TMOD_TR,
		.speed_hz = 12000000,
		.fifo_size = 32,
		.half_duplex = 1,
		.frame_header = 0xec,
	},
	{
		.slave = {
			.bus = 2,
			.rw = SPI_READ_FLAG | SPI_WRITE_FLAG,
		},
		.regs = (void *)SPI2_BASE_ADDR,
		.mode = 0,
		.bits_per_word = 8,
		.tmode = SPI_TMOD_TR,
		.speed_hz = 12000000,
		.fifo_size = 32,
		.half_duplex = 1,
		.frame_header = 0xec,
	},

};

static struct rockchip_spi_slave *to_rockchip_spi(struct spi_slave *slave)
{
	return container_of(slave, struct rockchip_spi_slave, slave);
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
	setbits_le32(&regs->SPIM_SER, 1);
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	struct rockchip_spi *regs = to_rockchip_spi(slave)->regs;
	clrbits_le32(&regs->SPIM_SER, 1);
}

static void rockchip_spi_enable_chip(struct rockchip_spi *regs, int enable)
{
	if (enable == 1)
		writel(1, &regs->SPIM_SPIENR);
	else
		writel(0, &regs->SPIM_SPIENR);
}


static void rockchip_spi_set_clk(struct rockchip_spi *regs,  unsigned short div)
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
	unsigned int src_clk;
	unsigned int src_clk_div;
	switch (slave->bus) { /*setting src clk div,now we set div=24*/
	case 0:
		g_cruReg->CRU_CLKSEL_CON[25] = (g_cruReg->CRU_CLKSEL_CON[25]
			& (~0x000000ff)) | 0x97;
		src_clk_div = g_cruReg->CRU_CLKSEL_CON[25]&0x7f;
		break;
	case 1:
		g_cruReg->CRU_CLKSEL_CON[25] = (g_cruReg->CRU_CLKSEL_CON[25]
			& (~0x0000ff00)) | 0x9700;
		src_clk_div = (g_cruReg->CRU_CLKSEL_CON[25]&0x7f00)>>8;
		break;
	case 2:
		g_cruReg->CRU_CLKSEL_CON[39] = (g_cruReg->CRU_CLKSEL_CON[39]
			& (~0x000000ff)) | 0x97;
		src_clk_div = g_cruReg->CRU_CLKSEL_CON[39]&0x7f;
		break;
	default:
		printk(BIOS_ERR, "do not support this spi bus\n");
	}
	rockchip_spi_enable_chip(regs, 0);
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
		printk(BIOS_DEBUG, "MRST SPI: unsupported bits: %dbits\n",
			espi->bits_per_word);
	}
	/* Calculate clock divisor.  */
	src_clk = rk_get_general_pll()/(src_clk_div + 1);
	clk_div = src_clk / espi->speed_hz;
	clk_div = (clk_div + 1) & 0xfffe;
	rockchip_spi_set_clk(regs, clk_div);
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

	/*fifi depth*/
	writel(espi->fifo_size/2 - 1, &regs->SPIM_TXFTLR);
	writel(espi->fifo_size/2 - 1, &regs->SPIM_RXFTLR);
	return 0;
}

static int rockchip_spi_wait_till_not_busy(struct rockchip_spi *regs)
{
	unsigned int delay = 1000;
	while (delay--) {
		if (!(readl(&regs->SPIM_SR) & SR_BUSY))
			return 0;
	}
	printk(BIOS_DEBUG, "RK SPI: Status keeps busy for 1000us after a read/write!\n");
	return -1;
}

int spi_xfer(struct spi_slave *slave, const void *dout, unsigned int bitsout,
	     void *din, unsigned int bitsin)
{
	unsigned int len;
	unsigned int ctrl;
	unsigned int bytes_remaining_to_be_transfered;
	uint8_t *p;
	struct rockchip_spi *regs = to_rockchip_spi(slave)->regs;
	struct rockchip_spi_slave *espi = to_rockchip_spi(slave);
	spi_cs_activate(slave);
	if (dout) {
		len = bitsout/8;
		p = (uint8_t *)dout;
		bytes_remaining_to_be_transfered = len;
		writel(0, &regs->SPIM_SPIENR); /*disable spi*/
		writel(len-1, &regs->SPIM_CTRLR1);/*wrtie len*/
		ctrl =  readl(&regs->SPIM_CTRLR0);
		ctrl = (ctrl & (~(3<<SPI_TMOD_OFFSET))) |
			(SPI_TMOD_TO << 18);/*tx only*/
		writel(ctrl, &regs->SPIM_CTRLR0);
		writel(1, &regs->SPIM_SPIENR); /*enable spi*/
		while (bytes_remaining_to_be_transfered) {
			if ((readl(&regs->SPIM_TXFLR) & 0x3f)
				< espi->fifo_size) {
				writel(*p++, &regs->SPIM_TXDR);
				bytes_remaining_to_be_transfered--;
			}
		}
		if (rockchip_spi_wait_till_not_busy(regs)) {
			spi_cs_deactivate(slave);
			return -1;
		}
	}
	if (din) {
		len = bitsin/8;
		p = (uint8_t *)din;
		writel(0, &regs->SPIM_SPIENR); /*disable spi*/
		writel(len-1, &regs->SPIM_CTRLR1);/*wrtie len*/
		ctrl =  readl(&regs->SPIM_CTRLR0);
		ctrl = (ctrl & (~(3<<SPI_TMOD_OFFSET))) |
			(SPI_TMOD_RO<<18);/*rx only*/
		writel(ctrl, &regs->SPIM_CTRLR0);
		while (len) {
			writel(0, &regs->SPIM_SPIENR); /*disable spi*/
			if (len > 0xffff) {
				bytes_remaining_to_be_transfered = 0xffff;
				writel(0xfffe, &regs->SPIM_CTRLR1);
				len = len - 0xffff;
			} else {
				bytes_remaining_to_be_transfered = len;
				writel(len-1,  &regs->SPIM_CTRLR1);
				len = 0;
			}
			writel(1, &regs->SPIM_SPIENR); /*enable spi*/
			while (bytes_remaining_to_be_transfered) {
				if (readl(&regs->SPIM_RXFLR) & 0x3f) {
					*p++ = readl(&regs->SPIM_RXDR) & 0xff;
					bytes_remaining_to_be_transfered--;
				}
			}
		}
		if (rockchip_spi_wait_till_not_busy(regs)) {
			spi_cs_deactivate(slave);
			return -1;
		}
	}
	spi_cs_deactivate(slave);
	return 0;
}
static int rockchip_spi_read(struct spi_slave *slave, void *dest, uint32_t len,
			   uint32_t off)
{
	unsigned int cmd;
	cmd = ((off & 0xff) << 24)|((off & 0xFF00) << 8)|
		((off & 0xFF0000) >> 8)|SF_READ_DATA_CMD;
	if (spi_xfer(slave, &cmd, sizeof(cmd)*8, dest, len*8))
		return -1;
	else
		return len;
}

void spi_release_bus(struct spi_slave *slave)
{

}

struct rockchip_spi_media {
	struct spi_slave *slave;
	struct cbfs_simple_buffer buffer;
};

static int rockchip_spi_cbfs_open(struct cbfs_media *media)
{
	struct rockchip_spi_media *spi =
		(struct rockchip_spi_media *)media->context;
	return spi_claim_bus(spi->slave);
}

static int rockchip_spi_cbfs_close(struct cbfs_media *media)
{
	struct rockchip_spi_media *spi =
		(struct rockchip_spi_media *)media->context;
	spi_release_bus(spi->slave);
	return 0;
}

static size_t rockchip_spi_cbfs_read(struct cbfs_media *media, void *dest,
				   size_t offset, size_t count)
{
	struct rockchip_spi_media *spi =
		(struct rockchip_spi_media *)media->context;
	int bytes;
	bytes = rockchip_spi_read(spi->slave, dest, count, offset);
	return bytes;
}

static void *rockchip_spi_cbfs_map(struct cbfs_media *media, size_t offset,
				 size_t count)
{
	struct rockchip_spi_media *spi =
		(struct rockchip_spi_media *)media->context;

	if (count % 4)
		count += 4 - (count % 4);

	return cbfs_simple_buffer_map(&spi->buffer, media, offset, count);
}

static void *rockchip_spi_cbfs_unmap(struct cbfs_media *media,
				   const void *address)
{
	struct rockchip_spi_media *spi =
		(struct rockchip_spi_media *)media->context;
	return cbfs_simple_buffer_unmap(&spi->buffer, address);
}

int initialize_rockchip_spi_cbfs_media(struct cbfs_media *media,
				     void *buffer_address,
				     size_t buffer_size)
{
	static struct rockchip_spi_media context;
	static struct rockchip_spi_slave *eslave = &rockchip_spi_slaves[2];
	context.slave = &eslave->slave;
	context.buffer.allocated = context.buffer.last_allocate = 0;
	context.buffer.buffer = buffer_address;
	context.buffer.size = buffer_size;
	media->context = (void *)&context;
	media->open = rockchip_spi_cbfs_open;
	media->close = rockchip_spi_cbfs_close;
	media->read = rockchip_spi_cbfs_read;
	media->map = rockchip_spi_cbfs_map;
	media->unmap = rockchip_spi_cbfs_unmap;

	return 0;
}
