#ifndef __RK3288_I2C_H__
#define __RK3288_I2C_H__

#define DIV_CEIL(x, y)							(((x) + (y) - 1) / y)

#define RK_I2C_CEIL(x, y) \
	  ({ unsigned long __x = (x), __y = (y); (__x + __y - 1) / __y; })


#define I2C_NOACK					2
#define I2C_TIMEOUT				3

#define Readl(addr)                     (*(volatile u32 *)(addr))
#define Writel(addr, v)               (*(volatile u32 *)(addr) = v)

#define I2C_CLK						(100 * 1000)  // 100khz

//
// Con register bits.
//
#define I2C_CON_OFF			0x00
#define I2C_ACT2NAK							0x40
#define I2C_NAK								0x20
#define I2C_STOP							0x10
#define I2C_START							0x08

#define I2C_MODE(mode)						((mode) <<1)
#define I2C_MODE_TX							0x00
#define I2C_MODE_TRX						0x01
#define I2C_MODE_RX							0x02
#define I2C_MODE_RRX						0x03

#define I2C_EN								0x01

//
// Clkdiv register bits.
//
#define I2C_CLKDIV_OFF		0x04
#define I2C_DIV(divh, divl)					(((divh)<<16) | (divl))

//
// Mrxaddr register bits.
//
#define I2C_MRXADDR_OFF		0x08
#define I2C_MRXADDR(vld, addr)				(((vld) << 24) | (addr))

//
// Mrxraddr register bits.
//

#define I2C_MRXRADDR_OFF	0x0c
#define I2C_MRXRADDR(vld, raddr)			(((vld) << 24) | (raddr))

//
// Mtxcnt register bits.
//
#define I2C_MTXCNT_OFF		0x10
#define I2C_MTXCNT(cnt)						((cnt) & 0x3F)

//
// Mrxcnt register bits.
//
#define I2C_MRXCNT_OFF		0x14
#define I2C_MRXCNT(cnt)						((cnt) & 0x3F)

//
// Ien register bits.
//
#define I2C_IEN_OFF			0x18
#define I2C_NAKRCVIEN						0x40
#define I2C_STOPIEN							0x20
#define I2C_STARTIEN						0x10
#define I2C_MBRFIEN							0x08
#define I2C_MBTFIEN							0x04
#define I2C_BRFIEN							0x02
#define I2C_BTFIEN							0x01

//
// Ipd register bits.
//
#define I2C_IPD_OFF			0x1c
#define I2C_NAKRCVIPD						0x40
#define I2C_STOPIPD							0x20
#define I2C_STARTIPD						0x10
#define I2C_MBRFIPD							0x08
#define I2C_MBTFIPD							0x04
#define I2C_BRFIPD							0x02
#define I2C_BTFIPD							0x01

#define I2C_CLEAN_IPDS						0x7F

//
// Fcnt register bits.
//
#define I2C_FCNT_OFF		0x20		
#define I2C_FCNT_MASK						0x3F	

// Txdata register
#define I2C_TXDATA0_OFF		0x100

// Rxdata register
#define I2C_RXDATA0_OFF		0x200

// TODO: Define other controller-specific values.

#define SI2C_MAX_TRANSFER_LENGTH            0x1000

// TODO: Remove these generic error defines in favor
//       of real register bit mappings defined above.

#define I2C_STAGE_IDLE						0x00000000
#define I2C_STAGE_SEND_START				0x00010000
#define I2C_STAGE_SEND_ADDR					0x00020000
#define I2C_STAGE_SEND_DATA					0x00040000
#define I2C_STAGE_RECV_DATA					0x00080000
#define I2C_STAGE_SEND_STOP					0x00100000

#define I2C_STATUS_ADDR_NACK				0x10000000
#define I2C_STATUS_DATA_NACK				0x20000000
#define I2C_STATUS_GENERIC_ERROR			0x40000000

#define I2C_STAGE_MASK						0xFFFF0000

#define I2C_GET_STAGE(x)				((x) & I2C_STAGE_MASK)


void i2c_init(unsigned bus);

#endif

