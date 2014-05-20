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
#include "reg.h"
#include "memap.h"
#include <console/console.h>
#include <timer.h>
#include <delay.h>
#include <stdint.h>


#define CONFIG_SYS_CLK_FREQ		24000000
#define TIMER_LOAD_VAL	0xffffffff
#define TIMER_FREQ	CONFIG_SYS_CLK_FREQ
#define CONFIG_SYS_HZ			1000

extern uint32_t  div64_32(uint64_t *n, uint32_t base);
extern inline unsigned long long tick_to_time(unsigned long long tick);
//static inline unsigned long get_rk_current_tick();

extern void __udelay(unsigned long usec);
uint32_t  div64_32(uint64_t *n, uint32_t base)
{
	uint64_t rem = *n;
	uint64_t b = base;
	uint64_t res, d = 1;
	uint32_t high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (uint64_t) high << 32;
		rem -= (uint64_t) (high*base) << 32;
	}

	while ((int64_t)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
}


/* The unnecessary pointer compare is there
 * to check for type safety (n must be 64bit)
 */
# define do_div(n,base) ({				\
	uint32_t __base = (base);			\
	uint32_t __rem;					\
	(void)(((typeof((n)) *)0) == ((uint64_t *)0));	\
	if (((n) >> 32) == 0) {			\
		__rem = (uint32_t)(n) % __base;		\
		(n) = (uint32_t)(n) / __base;		\
	} else						\
		__rem = div64_32(&(n), __base);	\
	__rem;						\
 })

inline unsigned long long tick_to_time(unsigned long long tick)
{
	tick *= CONFIG_SYS_HZ;
	do_div(tick, TIMER_FREQ);
	return tick;
}
static inline unsigned long long usec_to_tick(unsigned long long usec)
{
	usec *= TIMER_FREQ;
	do_div(usec, 1000000);
	return usec;
}
static inline unsigned long get_rk_current_tick(void)
{
    return g_Time7Reg->TIMER_CURR_VALUE0;
}

/* delay x useconds */
void __udelay(unsigned long usec)
{
    long long tmo;
    long long now, last;

    tmo = usec_to_tick(usec);

    /* get current timestamp */
    last = get_rk_current_tick();
    while (tmo > 0)	/* loop till event */
    {
        now = get_rk_current_tick();
        if (last >= now) {
            tmo -= last - now;
        } else {
            tmo -= 0xFFFFFFFF + last - now;
        }
        last = now;
    }
}


void udelay(unsigned int n) {
	/* TODO provide delay here. */
	__udelay(n);
}

void init_timer(void) {
	g_Time7Reg->TIMER_LOAD_COUNT0 = TIMER_LOAD_VAL;
	g_Time7Reg->TIMER_CTRL_REG = 0x01;
}
