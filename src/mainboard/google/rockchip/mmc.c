#include <delay.h>
#include <cbfs.h>
#include <string.h>
#include <console/console.h>

#include "Memap.h"
#include "mmc.h"

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
 int EmmcWrite (void *Buffer, unsigned int Offset, unsigned int BufferSize)
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
