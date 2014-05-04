/*
 * (C) Copyright 2008-2013 Rockchip Electronics
 * Peter, Software Engineering, <superpeter.cai@gmail.com>.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _RKXX_CLOCK_H
#define _RKXX_CLOCK_H


/* config cpu and general clock in MHZ */
#define KHZ				(1000)
#define MHZ				(1000*1000)


/*
 * rkplat clock set for arm and general pll
 */
void rkclk_set_pll(void);

int rk_get_arm_pll(void);

int rk_get_general_pll(void);

int rk_get_codec_pll(void);

int rk_get_ddr_pll(void);

int rk_get_new_pll(void);


#endif	/* _RKXX_CLOCK_H */

