/*
 * This file is part of the libpayload project.
 *
 * Copyright (C) 2008 Advanced Micro Devices, Inc.
 * Copyright (C) 2008 Ulf Jordan <jordan@chalmers.se>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <libpayload-config.h>
#include <libpayload.h>
#include "rk_serial.h"
#define UART2_BASE_ADDR            0xFF690000
pUART_REG pUartReg = (pUART_REG)UART2_BASE_ADDR;
#define ReadReg32(addr)                     (*(volatile u32 *)(addr))
#define WriteReg32(addr, data)              (*(volatile u32 *)(addr) = data)

static int serial_hardware_is_present = 0;
static int serial_is_mem_mapped = 0;

static uint8_t serial_read_reg(int offset)
{

}

static void serial_write_reg(uint8_t val, int offset)
{

}

#ifdef CONFIG_LP_SERIAL_SET_SPEED
static void serial_hardware_init(int speed, int word_bits,
				 int parity, int stop_bits)
{
	unsigned char reg;

}
#endif

static struct console_input_driver consin = {
	.havekey = &serial_havechar,
	.getchar = &serial_getchar
};

static struct console_output_driver consout = {
	.putchar = &serial_putchar
};

void serial_init(void)
{
	printf("in serial_init\n");
}

void serial_console_init(void)
{
	serial_init();
	console_add_input_driver(&consin);
	console_add_output_driver(&consout);
}

void serial_putchar(unsigned int c)
{
	u32 uartTimeOut;
	pUART_REG puartRegStart;
	if(!pUartReg) return;
	puartRegStart = (pUART_REG)pUartReg;
	uartTimeOut = 0xFFFF;
	while((puartRegStart->UART_USR & UART_TRANSMIT_FIFO_NOT_FULL) != UART_TRANSMIT_FIFO_NOT_FULL)
	{
		if(uartTimeOut == 0)
		{
			return ;
		}
		uartTimeOut--;
	}
	puartRegStart->UART_THR = c;
}

int serial_havechar(void)
{

	
}

int serial_getchar(void)
{
	
}
#define  UART_LSR_TEMT                0x40 /* Transmitter empty */

//#define UART2_BASE_ADDR 0xFF690000

int uart_wrtie_byte(char byte)
{
    pUART_REG puartRegStart = (pUART_REG)UART2_BASE_ADDR; 
    puartRegStart->UART_RBR = byte;
    while(!(puartRegStart->UART_LSR & UART_LSR_TEMT));
    return (0);
}

void print(char *s)
{
	while (*s) 
	{
		if (*s == '\n')
		{
		    uart_wrtie_byte('\r');
		}
	    uart_wrtie_byte(*s);
	    s++;
	}
}
void _print_hex (int hex)
{
    int i = 8;
	uart_wrtie_byte('0');
	uart_wrtie_byte('x');
	while (i--) {
		unsigned char c = (hex & 0xF0000000) >> 28;
		uart_wrtie_byte(c < 0xa ? c + '0' : c - 0xa + 'a');
		hex <<= 4;
	}
}

