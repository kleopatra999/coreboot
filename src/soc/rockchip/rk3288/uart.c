/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2013 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <console/console.h>
#include <uart.h>
#include "uart.h"
#include "reg.h"
#include "memap.h"


#define UART2_BASE_ADDR            0xFF690000
pUART_REG pUartReg = (pUART_REG)UART2_BASE_ADDR;
#define ReadReg32(addr)                     (*(volatile u32 *)(addr))
#define WriteReg32(addr, data)              (*(volatile u32 *)(addr) = data)

static void pl011_init_dev(void)
{
	u32  uartTemp;
	pUartReg->UART_SRR = UART_RESET | RCVR_FIFO_REST | XMIT_FIFO_RESET;
	pUartReg->UART_IER = 0;
	pUartReg->UART_MCR = IRDA_SIR_DISABLED;
	pUartReg->UART_LCR = LCR_DLA_EN | PARITY_DISABLED |
							ONE_STOP_BIT | LCR_WLS_8;
	uartTemp = (1000 * 24000) / MODE_X_DIV / 115200;
	pUartReg->UART_DLL = uartTemp & 0xff;
	pUartReg->UART_DLH = (uartTemp>>8) & 0xff;
	pUartReg->UART_LCR = PARITY_DISABLED | ONE_STOP_BIT | LCR_WLS_8;
	pUartReg->UART_SFE = SHADOW_FIFI_ENABLED;
	pUartReg->UART_SRT = RCVR_TRIGGER_TWO_LESS_FIFO;
	pUartReg->UART_STET = TX_TRIGGER_TWO_IN_FIFO;
}

static void pl011_uart_tx_byte(unsigned char data)
{
	u32 uartTimeOut;
	pUART_REG puartRegStart;
	if (!pUartReg)
		return;
	puartRegStart = (pUART_REG)pUartReg;
	uartTimeOut = 0xFFFF;
	while ((puartRegStart->UART_USR & UART_TRANSMIT_FIFO_NOT_FULL)
			!= UART_TRANSMIT_FIFO_NOT_FULL) {
		if (uartTimeOut == 0)
			return;
		uartTimeOut--;
	}
	puartRegStart->UART_THR = data;
}

static void pl011_uart_tx_flush(void)
{
}
#if !defined(__PRE_RAM__)
static const struct console_driver pl011_uart_console __console = {
	.init     = pl011_init_dev,
	.tx_byte  = pl011_uart_tx_byte,
	.tx_flush = pl011_uart_tx_flush,
};

u32 uartmem_getbaseaddr(void)
{
	return UART2_BASE_ADDR;
}
#else
u32 uartmem_getbaseaddr(void)
{
	return UART2_BASE_ADDR;
}

void uart_init(void)
{
	pl011_init_dev();
}

void uart_tx_byte(unsigned char data)
{
	pl011_uart_tx_byte(data);
}

void uart_tx_flush(void)
{
	pl011_uart_tx_flush();
}
#endif
