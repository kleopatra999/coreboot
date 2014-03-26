#include <delay.h>
#include <cbfs.h>
#include <string.h>
#include <console/console.h>

#include "Memap.h"

#define BIT(nr)			(1UL << (nr))

#define	MMC_CTRL	0x00
#define MMC_PWREN	0X04
#define MMC_CLKDIV	0x08
#define MMC_CLKSRC	0x0c
#define MMC_CLKENA	0x10
#define MMC_TMOUT	0x14
#define MMC_CTYPE	0x18
#define MMC_BLKSIZ	0x1c
#define MMC_BYTCNT	0x20
#define MMC_INTMASK	0x24
#define MMC_CMDARG	0x28
#define MMC_CMD         0x2c
#define MMC_RESP0	0x30
#define MMC_RESP1	0X34
#define MMC_RESP2	0x38
#define MMC_RESP3	0x3c
#define MMC_MINTSTS	0x40
#define MMC_RINTSTS	0x44
#define MMC_STATUS	0x48
#define MMC_FIFOTH	0x4c
#define MMC_CDETECT	0x50
#define MMC_WRTPRT	0x54
#define MMC_TCBCNT	0x5c
#define MMC_TBBCNT	0x60
#define MMC_DEBNCE	0x64
#define MMC_USRID	0x68
#define MMC_VERID	0x6c
#define MMC_UHS_REG	0X74
#define MMC_RST_N	0x78

#define MAX_RETRY_COUNT (250000)  //250ms
#define MMC_FIFO_BASE	0x200
#define MMC_DATA	MMC_FIFO_BASE
/* Control register defines */
#define MMC_CTRL_ABORT_READ_DATA	BIT(8)
#define MMC_CTRL_SEND_IRQ_RESPONSE	BIT(7)
#define MMC_CTRL_READ_WAIT		BIT(6)
#define MMC_CTRL_DMA_ENABLE		BIT(5)
#define MMC_CTRL_INT_ENABLE		BIT(4)
#define MMC_CTRL_DMA_RESET		BIT(2)
#define MMC_CTRL_FIFO_RESET		BIT(1)
#define MMC_CTRL_RESET			BIT(0)
/* Hardware reset register defines */
#define MMC_CARD_RESET			BIT(0)
/* Power enable register defines */
#define MMC_PWREN_ON			BIT(0)
/* Clock Enable register defines */
#define MMC_CLKEN_LOW_PWR             	BIT(16)
#define MMC_CLKEN_ENABLE              	BIT(0)
/* time-out register defines */
#define MMC_TMOUT_DATA(n)             	_SBF(8, (n))
#define MMC_TMOUT_DATA_MSK            	0xFFFFFF00
#define MMC_TMOUT_RESP(n)             	((n) & 0xFF)
#define MMC_TMOUT_RESP_MSK            	0xFF
/* card-type register defines */
#define MMC_CTYPE_8BIT                	BIT(16)
#define MMC_CTYPE_4BIT                	BIT(0)
#define MMC_CTYPE_1BIT                	0
/* Interrupt status & mask register defines */
#define MMC_INT_SDIO                  	BIT(16)
#define MMC_INT_EBE                   	BIT(15)
#define MMC_INT_ACD                   	BIT(14)
#define MMC_INT_SBE                   	BIT(13)
#define MMC_INT_HLE                   	BIT(12)
#define MMC_INT_FRUN                  	BIT(11)
#define MMC_INT_HTO                   	BIT(10)
#define MMC_INT_DTO                   	BIT(9)
#define MMC_INT_RTO                   	BIT(8)
#define MMC_INT_DCRC                  	BIT(7)
#define MMC_INT_RCRC                  	BIT(6)
#define MMC_INT_RXDR                  	BIT(5)
#define MMC_INT_TXDR                  	BIT(4)
#define MMC_INT_DATA_OVER             	BIT(3)
#define MMC_INT_CMD_DONE              	BIT(2)
#define MMC_INT_RESP_ERR              	BIT(1)
#define MMC_INT_CD                    	BIT(0)
#define MMC_INT_ERROR                 	0xbfc2
/* Command register defines */
#define MMC_CMD_START                 	BIT(31)
#define MMC_USE_HOLD_REG		BIT(29)
#define MMC_CMD_CCS_EXP               	BIT(23)
#define MMC_CMD_CEATA_RD              	BIT(22)
#define MMC_CMD_UPD_CLK               	BIT(21)
#define MMC_CMD_INIT                  	BIT(15)
#define MMC_CMD_STOP                  	BIT(14)
#define MMC_CMD_PRV_DAT_WAIT          	BIT(13)
#define MMC_CMD_SEND_STOP             	BIT(12)
#define MMC_CMD_STRM_MODE             	BIT(11)
#define MMC_CMD_DAT_WR                	BIT(10)
#define MMC_CMD_DAT_EXP               	BIT(9)
#define MMC_CMD_RESP_CRC              	BIT(8)
#define MMC_CMD_RESP_LONG		BIT(7)
#define MMC_CMD_RESP_EXP		BIT(6)
#define MMC_CMD_INDX(n)		        ((n) & 0x1F)

/* Status register defines */
#define MMC_GET_FCNT(x)		        (((x)>>17) & 0x1FF)
#define MMC_MC_BUSY			BIT(10)
#define MMC_DATA_BUSY			BIT(9)
#define MMC_BUSY			(MMC_MC_BUSY | MMC_DATA_BUSY)
/* FIFO threshold register defines */
#define FIFO_DETH			512

/* UHS-1 register defines */
#define MMC_UHS_DDR_MODE		BIT(16)
#define MMC_UHS_VOLT_18			BIT(0)


/* Common flag combinations */
#define MMC_DATA_ERROR_FLAGS (MMC_INT_DTO | MMC_INT_DCRC | \
		                                 MMC_INT_HTO | MMC_INT_SBE  | \
		                                 MMC_INT_EBE)
#define MMC_CMD_ERROR_FLAGS  (MMC_INT_RTO | MMC_INT_RCRC | \
		                                 MMC_INT_RESP_ERR)
#define MMC_ERROR_FLAGS      (MMC_DATA_ERROR_FLAGS | \
		                                 MMC_CMD_ERROR_FLAGS  | MMC_INT_HLE)


#define Readl(addr)                     (*(volatile u32 *)(addr))
#define Writel(addr, v)              	(*(volatile u32 *)(addr) = v)

#define BLKSZ 0x200
#define gMmcBaseAddr  EMMC_BASE_ADDR


#define emmc_dbg(x...)           printk(BIOS_DEBUG, ## x)
#define emmc_info(x...)          printk(BIOS_INFO, ## x)
#define emmc_err(x...)           printk(BIOS_ERR, ## x)


typedef enum {
  READ = 0,
  WRITE,
} OPERATION_TYPE;

static int MmcWaitBusy(void)
{
  int count;
  /* Wait max 100 ms */
  count = MAX_RETRY_COUNT;
  /* before reset ciu, it should check DATA0. if when DATA0 is low and
     it resets ciu, it might make a problem */
  while ((Readl ((gMmcBaseAddr + MMC_STATUS)) & MMC_BUSY)){
    if(count == 0){
      return -1;
    }
    count--;
    udelay(1);
  }
  return 0;
}

static int EmmcSendCmd (unsigned int Cmd, unsigned int Arg)
{
  volatile unsigned int RetryCount = 0;

  Writel((gMmcBaseAddr + MMC_CMDARG), Arg);
  Writel((gMmcBaseAddr + MMC_CMD), Cmd);

 udelay(1);
  for (RetryCount; RetryCount<MAX_RETRY_COUNT; RetryCount++) {
    if(Readl(gMmcBaseAddr + MMC_RINTSTS) & MMC_INT_CMD_DONE){
      Writel(gMmcBaseAddr + MMC_RINTSTS, MMC_INT_CMD_DONE);
      break;
    }
    udelay(1);
  }
  if (RetryCount == MAX_RETRY_COUNT) {
    emmc_err("Emmc::EmmcSendCmd failed, Cmd: 0x%08x, Arg: 0x%08x\n", Cmd, Arg);
    return -1;
  }
  if(Readl(gMmcBaseAddr + MMC_RINTSTS) & MMC_CMD_ERROR_FLAGS) {
    emmc_err("Emmc::EmmcSendCmd error, Cmd: 0x%08x, Arg: 0x%08x, RINTSTS: 0x%08x\n",
			    Cmd, Arg, (Readl(gMmcBaseAddr + MMC_RINTSTS)));
    Writel(gMmcBaseAddr + MMC_RINTSTS, MMC_CMD_ERROR_FLAGS);
    return -1;
  }

  return 0;
}
/*
static int EmmcSetTransferSize (unsigned int Blocks)
{
  return EmmcSendCmd(0xa0002157, Blocks);
}
*/
static int EmmcPreTransfer (unsigned int Blocks)
{
  int Status;
  volatile unsigned int RetryCount = 0;

  while ((RetryCount++ < MAX_RETRY_COUNT) && (Readl ((gMmcBaseAddr + MMC_STATUS)) & MMC_BUSY))
    udelay(1);
    if (RetryCount == MAX_RETRY_COUNT) {
      Status = -1;
      emmc_err("Emmc::EmmcPreTransfer: mmc is busy\n");
      return Status;
    }

  Writel(gMmcBaseAddr + MMC_CTRL, Readl(gMmcBaseAddr + MMC_CTRL) | MMC_CTRL_FIFO_RESET);
  Writel(gMmcBaseAddr + MMC_TMOUT, 0xffffffff);
  Writel(gMmcBaseAddr + MMC_BYTCNT, Blocks *BLKSZ);
  Writel(gMmcBaseAddr + MMC_BLKSIZ, BLKSZ);
  Writel((gMmcBaseAddr + MMC_INTMASK), 
		  MMC_INT_TXDR | MMC_INT_RXDR | MMC_INT_CMD_DONE | MMC_INT_DATA_OVER | MMC_ERROR_FLAGS);
  Writel((gMmcBaseAddr + MMC_FIFOTH), (0x3 << 28) | ((FIFO_DETH/2 - 1) << 16) | ((FIFO_DETH/2) << 0));

  Writel(gMmcBaseAddr + MMC_CTRL, Readl(gMmcBaseAddr + MMC_CTRL) | MMC_CTRL_INT_ENABLE);
  //Wait contrloler ready
  Status = MmcWaitBusy();
  if (Status < 0) {
      emmc_err("Emmc::EmmcPreTransfer failed, data busy\n");
      return Status;
  }
  //Clean all interrupt
  Writel((gMmcBaseAddr + MMC_RINTSTS), 0xffffffff);

  return 0;
}

static int EmmcWriteData (void *Buffer, unsigned int Blocks)
{
  unsigned int *DataBuffer = Buffer;
  unsigned int Count=0;
  unsigned int Size32 = Blocks * BLKSZ/4;

  for (Count = 0; Count < Size32; Count++)
    Writel((gMmcBaseAddr + MMC_DATA), *DataBuffer++);

  return 0;
}

static int EmmcReadData (void *Buffer, unsigned int Blocks)
{
  int Status;
  unsigned int *DataBuffer = Buffer;
  unsigned int FifoCount=0;
  unsigned int Count=0;
  unsigned int Size32 = Blocks * BLKSZ / 4;

  while(Size32){
    if(Readl(gMmcBaseAddr + MMC_RINTSTS) & MMC_INT_RXDR) {
      FifoCount = MMC_GET_FCNT(Readl(gMmcBaseAddr + MMC_STATUS));

      for (Count = 0; Count < FifoCount; Count++)
        *DataBuffer++ = Readl(gMmcBaseAddr + MMC_DATA);

      Writel(gMmcBaseAddr + MMC_RINTSTS, MMC_INT_RXDR);
      Size32 -= FifoCount;
    }
    if(Readl(gMmcBaseAddr + MMC_RINTSTS) & MMC_INT_DATA_OVER) {
      FifoCount = MMC_GET_FCNT(Readl(gMmcBaseAddr + MMC_STATUS));

      for (Count = 0; Count < FifoCount; Count++){
        *DataBuffer++ = Readl(gMmcBaseAddr + MMC_DATA);
      }

      Writel(gMmcBaseAddr + MMC_RINTSTS, MMC_INT_DATA_OVER);
      Size32 -= FifoCount;
    }
    if(Readl(gMmcBaseAddr + MMC_RINTSTS) & MMC_DATA_ERROR_FLAGS) {
      emmc_err("Emmc::ReadSingleBlock data error, RINTSTS: 0x%08x\n",
			    (Readl(gMmcBaseAddr + MMC_RINTSTS)));
      Writel(gMmcBaseAddr + MMC_RINTSTS, MMC_DATA_ERROR_FLAGS);
      return -1;
    }
  }
  if(Size32 == 0)
    Status = 0;
  else
    Status = -1;

  return Status;
}

static int EmmcTransferBlocks (void *Buffer, 
			unsigned int Blocks, 
			unsigned int Lba, 
			OPERATION_TYPE OperationType)
{
  int Status;

  Status = EmmcPreTransfer(Blocks);
  if (Status < 0) 
    return Status;

  if(Blocks == 1){
    if(OperationType == READ)
      Status = EmmcSendCmd(0xa0002351, Lba);
    else
      Status = EmmcSendCmd(0xa0002758, Lba);

    if (Status < 0)
      goto Done;
  }else{
#if 0
    Status = EmmcSetTransferSize(Blocks);
    if (Status < 0)
      goto Done;
#endif
    if(OperationType == READ)
      Status = EmmcSendCmd(0xa0002352, Lba);
    else
      Status = EmmcSendCmd(0xa0002759, Lba);

    if (Status < 0)
      goto Done;
  }

  if(OperationType == READ)
    Status = EmmcReadData(Buffer, Blocks);
  else
    Status = EmmcWriteData(Buffer, Blocks);
  if(Blocks >1)
    Status = EmmcSendCmd(0xa000414c, 0);

Done:
  return Status;
}

/*
 * Function: EmmcRead 
 *
 * parameter:
 * 1.Buffer -- Buffer point address must be divisible by 4
 * 2.Offset -- Absolute offset byte, must be divisible by 512(block size)
 * 3.BufferSize -- Buffser size in byte, must be divisible by 512(block size) 
 *
 * Return value: Return negative errno on error, 0 on success
 */
static int EmmcRead (void *Buffer, unsigned int Offset, unsigned int BufferSize)
{
  if(Buffer == NULL)
    return -1;
  if((unsigned int)Buffer%4 != 0)
    return -1;
  if(BufferSize%BLKSZ != 0)
    return -1;
  if(Offset%BLKSZ != 0)
    return -1;
  
  emmc_info("Emmc::EmmcRead : Offset = 0x%x, BufferSize = 0x%x, Buffer = 0x%x\n",
		Offset, BufferSize, (unsigned int)Buffer);
  return EmmcTransferBlocks(Buffer, BufferSize/BLKSZ, Offset/BLKSZ, READ);

}

/*
 * Function: EmmcWrite 
 *
 * parameter:
 * 1.Buffer -- Buffer point address must be divisible by 4
 * 2.Offset -- Absolute offset byte, must be divisible by 512(block size)
 * 3.BufferSize -- Buffser size in byte, must be divisible by 512(block size) 
 *
 * Return value: Return negative errno on error, 0 on success
 *
 * Note: Only support single write now
 */
static int EmmcWrite (void *Buffer, unsigned int Offset, unsigned int BufferSize)
{
  if(Buffer == NULL)
    return -1;
  if((unsigned int)Buffer%4 != 0)
    return -1;
  // only support single write now
  if(BufferSize != BLKSZ)
    return -1;
  if(Offset%BLKSZ != 0)
    return -1;
  
  emmc_info("Emmc::EmmcWrite : Offset = 0x%x, BufferSize = 0x%x, Buffer = 0x%x\n",
		Offset, BufferSize, (unsigned int)Buffer);
  return EmmcTransferBlocks(Buffer, BufferSize/BLKSZ, Offset/BLKSZ, WRITE);

}
