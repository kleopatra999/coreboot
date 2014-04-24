#include <console/console.h>
#include <delay.h>
//#include <arch/io.h>
#include <cbfs.h>
#include <string.h>
#include <device/i2c.h>

#include "Memap.h"
#include "i2c.h"

#define RETRY_COUNT	3
#define I2C_TIMEOUT_US	100000  // 100000us = 100ms
#if 0
#define i2c_info(x...)          printk(BIOS_INFO, ## x)
#else
#define i2c_info(x...)
#endif

static void ShowRegVal (
  unsigned int BaseAddr
  )
{
  i2c_info("I2C_CON_OFF = 0x%08x\n", Readl(BaseAddr + I2C_CON_OFF));
  i2c_info("I2C_CLKDIV_OFF = 0x%08x\n", Readl(BaseAddr + I2C_CLKDIV_OFF));
  i2c_info("I2C_MRXADDR_OFF = 0x%08x\n", Readl(BaseAddr + I2C_MRXADDR_OFF));
  i2c_info("I2C_MTXCNT_OFF = 0x%08x\n", Readl(BaseAddr + I2C_MTXCNT_OFF));
  i2c_info("I2C_MRXCNT_OFF  = 0x%08x\n", Readl(BaseAddr + I2C_MRXCNT_OFF));
  i2c_info("I2C_IEN_OFF  = 0x%08x\n", Readl(BaseAddr + I2C_IEN_OFF));
  i2c_info("I2C_IPD_OFF  = 0x%08x\n", Readl(BaseAddr + I2C_IPD_OFF));
  i2c_info("I2C_FCNT_OFF  = 0x%08x\n", Readl(BaseAddr + I2C_FCNT_OFF));
  i2c_info("I2C_TXDATA0_OFF  = 0x%08x\n", Readl(BaseAddr + I2C_TXDATA0_OFF));
  i2c_info("I2C_RXDATA0_OFF  = 0x%08x\n", Readl(BaseAddr + I2C_RXDATA0_OFF));
}

static unsigned int GetBaseAddressByBus (
  unsigned Bus
  )
{
  switch (Bus) {
  case 0:
    return I2C0_BASE_ADDR;
  case 1:
    return I2C1_BASE_ADDR;
  case 2:
    return I2C2_BASE_ADDR;
  case 3:
    return I2C3_BASE_ADDR;
  case 4:
    return I2C4_BASE_ADDR;
  case 5:
    return I2C5_BASE_ADDR;
  default:
    return 0;
  }
}

static void
RK3288I2cSetBusClockHertz (
  unsigned 					  	 Bus,
  unsigned int           BusClockHertz
  )
{
  unsigned int Div, DivL, DivH;
  unsigned int ClkSrcHz=0;
  unsigned int BaseAddr;

  BaseAddr = GetBaseAddressByBus(Bus);
  
  ClkSrcHz = 32000000;

  Div = RK_I2C_CEIL(ClkSrcHz,BusClockHertz*8);
  DivH = DivL = RK_I2C_CEIL(Div, 2);

  Writel(BaseAddr + I2C_CLKDIV_OFF, I2C_DIV(DivL,DivH));
}

static int
RK3288I2cSendStartBit (
  unsigned int 	      BaseAddr
  )
{
  int  Status = 0;
  int TimeOut = I2C_TIMEOUT_US;
	
  i2c_info("I2c Start::Send Start bit\n");
  Writel(BaseAddr + I2C_IPD_OFF, I2C_CLEAN_IPDS);

  Writel(BaseAddr + I2C_CON_OFF, I2C_EN | I2C_START);
  Writel(BaseAddr + I2C_IEN_OFF, I2C_STARTIEN);


  while(TimeOut--) {
    if(Readl(BaseAddr + I2C_IPD_OFF) & I2C_STARTIPD){
      Writel(BaseAddr + I2C_IPD_OFF, I2C_STARTIPD);
      break;
    }
    udelay(1);
  }
  if(TimeOut <= 0) {
    printk(BIOS_ERR, "I2C Start::Send Start Bit Timeout\n");
    ShowRegVal(BaseAddr);
    Status = I2C_TIMEOUT;
  }
  
  return Status;
}

static int
RK3288I2cSendStopBit (
  unsigned int 	      BaseAddr
  )
{
  int  Status = 0;
  int TimeOut = I2C_TIMEOUT_US;
	
  i2c_info("I2c Stop::Send Stop bit\n");
  Writel(BaseAddr + I2C_IPD_OFF, I2C_CLEAN_IPDS);

  Writel(BaseAddr + I2C_CON_OFF, I2C_EN | I2C_STOP);
  Writel(BaseAddr + I2C_IEN_OFF, I2C_STOPIEN);

  while(TimeOut--) {
    if(Readl(BaseAddr + I2C_IPD_OFF) & I2C_STOPIPD){
      Writel(BaseAddr + I2C_IPD_OFF, I2C_STOPIPD);
      break;
    }
    udelay(1);
  }
  if(TimeOut <= 0) {
    printk(BIOS_ERR, "I2C Stop::Send Stop Bit Timeout\n");
    ShowRegVal(BaseAddr);
    Status = I2C_TIMEOUT;
  }
  
  return Status;
}

static void
RK3288I2cDisable (
  unsigned int 	      BaseAddr
  )
{
  Writel(BaseAddr + I2C_CON_OFF, 0);
}


static int
RK3288I2cRead (
  unsigned int 	  BaseAddr,
	unsigned 				chip, 
	unsigned 				addr,
  unsigned 				alen, 
	uint8_t 				*buf, 
	unsigned 				len,
  unsigned int	  RetryCount
  )
{
  int  Status = 0;
  uint8_t *p = buf;
  int TimeOut = I2C_TIMEOUT_US;
  unsigned int BytesRemainingToBeTransfered = len;
  unsigned int BytesToBeTranfered = 0;
  unsigned int WordsToBeTranfered = 0;
  unsigned int Rxdata;
  unsigned int Con;
  unsigned int i, j; 

  if(buf == NULL)
      return 1;

  // send start bit
  Status = RK3288I2cSendStartBit(BaseAddr);
	if(Status != 0)
		return Status != 0;

  Writel(BaseAddr + I2C_MRXADDR_OFF, I2C_MRXADDR(1, chip << 1 | 1));
  if(alen == 0)
    Writel(BaseAddr +  I2C_MRXRADDR_OFF, 0);
	else if(alen < 4)
    Writel(BaseAddr +  I2C_MRXRADDR_OFF, I2C_MRXRADDR(alen, addr));
  else{
			printk(BIOS_ERR, "I2C Read: addr len %d not supported\n", alen);
      return 1;
	}

  while(BytesRemainingToBeTransfered){
    if(BytesRemainingToBeTransfered > 32){
      Con = I2C_EN | I2C_MODE(I2C_MODE_TRX);
      BytesToBeTranfered = 32;
    }else{
      BytesToBeTranfered = BytesRemainingToBeTransfered;
      Con = I2C_EN | I2C_MODE(I2C_MODE_TRX) | I2C_NAK;
    }
    WordsToBeTranfered = DIV_CEIL(BytesToBeTranfered, 4);

    Writel(BaseAddr + I2C_CON_OFF, Con);
    Writel(BaseAddr + I2C_MRXCNT_OFF, BytesToBeTranfered);
    Writel(BaseAddr + I2C_IEN_OFF, I2C_MBRFIPD | I2C_NAKRCVIEN);
  
    while(TimeOut--){
      if(Readl(BaseAddr + I2C_IPD_OFF) & I2C_NAKRCVIPD){
        Writel(BaseAddr + I2C_IPD_OFF, I2C_NAKRCVIPD);

	      if(RetryCount == RETRY_COUNT)
          printk(BIOS_ERR, "I2C Read::Received no ack, Retry %d...\n", RetryCount);

        Status = I2C_NOACK;
      }
      if(Readl(BaseAddr + I2C_IPD_OFF) & I2C_MBRFIPD){
        Writel(BaseAddr + I2C_IPD_OFF, I2C_MBRFIPD);
        break;
      }
      udelay(1);
    }
    if(TimeOut <= 0) {
      printk(BIOS_ERR, "I2C Read::Recv Data Timeout\n");
      ShowRegVal(BaseAddr);
      Status =  I2C_TIMEOUT;
      goto Exit;
    }

    for (i = 0; i < WordsToBeTranfered; i++){
      Rxdata = Readl(BaseAddr + I2C_RXDATA0_OFF + i * 4);
      i2c_info("I2c Read::RXDATA[%d] = 0x%x\n", i, Rxdata);
      for (j = 0; j < 4; j++) {
        if((i * 4 + j) == BytesToBeTranfered)
	  			break;
				*p++ = (Rxdata>>(j * 8)) & 0xff; 
      }
    }
    BytesRemainingToBeTransfered -= BytesToBeTranfered;
    i2c_info("I2C Read::BytesRemainingToBeTransfered %d\n", BytesRemainingToBeTransfered);
  }
Exit:
	// Send stop bit
	if(Status != 0)
	  RK3288I2cSendStopBit(BaseAddr);
	else
	  Status = RK3288I2cSendStopBit(BaseAddr);
	// Disable Controller
	RK3288I2cDisable(BaseAddr);

  return Status;
}

static int
RK3288I2cWrite (
  unsigned int 	  BaseAddr,
	unsigned 				chip, 
	unsigned 				addr,
  unsigned 				alen, 
	const uint8_t 	*buf, 
	unsigned 				len,
  unsigned int	  RetryCount
  )
{
  int  Status = 0;
  uint8_t *p = (uint8_t *)buf;
  int TimeOut = I2C_TIMEOUT_US;
  unsigned int BytesRemainingToBeTransfered = len + alen + 1;
  unsigned int BytesToBeTranfered = 0;
  unsigned int WordsToBeTranfered = 0;
  unsigned int Txdata;
  unsigned int i, j; 

  if(buf == NULL)
      return 1;

  // send start bit
  Status = RK3288I2cSendStartBit(BaseAddr);
	if(Status != 0)
		return Status != 0;

  while(BytesRemainingToBeTransfered){
    if(BytesRemainingToBeTransfered > 32)
      BytesToBeTranfered = 32;
    else
      BytesToBeTranfered = BytesRemainingToBeTransfered;

    WordsToBeTranfered = DIV_CEIL(BytesToBeTranfered, 4);

    for (i = 0; i < WordsToBeTranfered; i++){
      Txdata = 0;
      for (j = 0; j < 4; j++) {
        if((i * 4 + j) == BytesToBeTranfered)
	  			break;
				if(i == 0 && j == 0)
	  			Txdata |= (chip << 1);
				else if(i == 0 && j <= alen)
					Txdata |= (addr & (0xff<<((j - 1) * 8))) << 8; 
				else{
	 				Txdata |= (*p++)<<(j * 8);
				}

    		Writel(BaseAddr + I2C_TXDATA0_OFF + i * 4, Txdata);
      }
      i2c_info("I2c Write::TXDATA[%d] = 0x%x\n", i, Txdata);
    }

    Writel(BaseAddr + I2C_CON_OFF, I2C_EN | I2C_MODE(I2C_MODE_TX));
    Writel(BaseAddr + I2C_MTXCNT_OFF, BytesToBeTranfered);
    Writel(BaseAddr + I2C_IEN_OFF, I2C_MBTFIPD | I2C_NAKRCVIEN);

    while(TimeOut--){
      if(Readl(BaseAddr + I2C_IPD_OFF) & I2C_NAKRCVIPD){
        Writel(BaseAddr + I2C_IPD_OFF, I2C_NAKRCVIPD);
			if(RetryCount == RETRY_COUNT)
        printk(BIOS_ERR, "I2C Write::Received no ack, Retry %d...\n", RetryCount);
        Status = I2C_NOACK;
      }
      if(Readl(BaseAddr + I2C_IPD_OFF) & I2C_MBTFIPD){
        Writel(BaseAddr + I2C_IPD_OFF, I2C_MBTFIPD);
        break;
      }
      udelay(1);
    }
    if(TimeOut <= 0) {
      printk(BIOS_ERR, "I2C Write::Send Data Timeout\n");
      ShowRegVal(BaseAddr);
      Status =  I2C_TIMEOUT;
      goto Exit;
    }
    BytesRemainingToBeTransfered -= BytesToBeTranfered;
    i2c_info("I2C Write::BytesRemainingToBeTransfered %d\n", BytesRemainingToBeTransfered);
  }
Exit:
	// Send stop bit
	if(Status != 0)
	  RK3288I2cSendStopBit(BaseAddr);
	else
	  Status = RK3288I2cSendStopBit(BaseAddr);
	// Disable Controller
	RK3288I2cDisable(BaseAddr);

  return Status;
}

int i2c_read(
	unsigned 				bus, 
	unsigned 				chip, 
	unsigned 				addr,
  unsigned 				alen, 
	uint8_t 				*buf, 
	unsigned 				len
	)
{
	int Status;
  unsigned int RetryCount = 0;
	unsigned int BaseAddr = 0;

  i2c_info("I2c Read::Bus %d, BaseAddr 0x%08x\n", bus, BaseAddr);

	BaseAddr = GetBaseAddressByBus(bus);

  // set bus clock
  RK3288I2cSetBusClockHertz(bus, I2C_CLK);

  // clean all ipd
  Writel(BaseAddr + I2C_IPD_OFF, I2C_CLEAN_IPDS);

  while(RetryCount++ < RETRY_COUNT){
    Status = RK3288I2cRead(BaseAddr, chip, addr, alen, buf, len, RetryCount);
    if(Status != I2C_NOACK)
	    break;
	}

	return Status != 0;
}

int i2c_write(
	unsigned 				bus, 
	unsigned 				chip, 
	unsigned 				addr,
  unsigned 				alen, 
	const uint8_t 	*buf, 
	unsigned 				len
	)
{
	int Status;
  unsigned int RetryCount = 0;
	unsigned int BaseAddr = 0;

  i2c_info("I2c Write::Bus %d, BaseAddr 0x%08x\n", bus, BaseAddr);

	BaseAddr = GetBaseAddressByBus(bus);

  // set bus clock
  RK3288I2cSetBusClockHertz(bus, I2C_CLK);

  // clean all ipd
  Writel(BaseAddr + I2C_IPD_OFF, I2C_CLEAN_IPDS);

  while(RetryCount++ < RETRY_COUNT){
    Status = RK3288I2cWrite(BaseAddr, chip, addr, alen, buf, len, RetryCount);
    if(Status != I2C_NOACK)
	    break;
	}

	return Status != 0;
}

static void i2c_test(unsigned bus)
{
#if 0
  uint8_t buf[2];
  
	buf[0] = 0x35;
	i2c_write(bus, 0x6b, 0x00, 1, buf, 1);
	buf[0] = 0x00;
  i2c_read(bus, 0x6b, 0x00, 1, buf, 1);
	printk(BIOS_ERR, "Write Value: 0x35, Read Value: 0x%x\n", buf[0]); 
	buf[0] = 0x37;
	i2c_write(bus, 0x6b, 0x00, 1, buf, 1);
	buf[0] = 0x00;
  i2c_read(bus, 0x6b, 0x00, 1, buf, 1);
	printk(BIOS_ERR, "Write Value: 0x37, Read Value: 0x%x\n", buf[0]); 
#endif
}
void i2c_init(unsigned bus)
{
	// set i2c iomux
	switch(bus){
		case 0:
			Writel(0xFF730088, 0x00004000);
			Writel(0xFF73008C, 0x00000001);
	    i2c_test(bus);
			break;
		case 1:
			Writel(0xFF770080, 0x03000100);
			Writel(0xFF770080, 0x0C000400);
			break;
		case 2:
			Writel(0xFF770060, 0x000C0004);
			Writel(0xFF770060, 0x00300010);
			break;
		case 3:
			Writel(0xFF770018, 0x00030001);
			Writel(0xFF770018, 0x000C0004);
			break;
		case 4:
			Writel(0xFF770074, 0x00F00010);
			Writel(0xFF770074, 0x0F000100);
			break;
		case 5:
			Writel(0xFF770074, 0xF0001000);
			Writel(0xFF770078, 0x000F0001);
			break;
		default:
			break;
	}
   
}


