/*
 * Copyright (C) 2014
 * zyw <zyw@rock-chips.com>
 *
 */
#include <console/console.h>
#include <arch/io.h>

#include "gpio.h"


#if 1
#define gpio_err(x...)          printk(BIOS_INFO, ## x)
#else
#define gpio_err(x...)
#endif

#define gpio_readl(addr)	readl((const void *)addr)
#define gpio_writel(v, addr)	writel(v, (int*)addr)

/*
 * It's how we calculate the full port address
 * We have to get the group number from gpio
 * So we set the base addr to high 16bit of gpio
 * eg:gpio=0xFF780008, it mean gpio1_B0, 0xFF780000 is base addr of GPIO1, 0x8 is gpio_b0 
 */

int gpio_set_value(unsigned gpio, int value)
{
	int reg_addr = gpio&0xffff0000;
    int index = gpio&0xffff;
    if(reg_addr==0)
    {
        gpio_err("no gpio group \n");
        return -1;
    }
    if(value)gpio_writel(gpio_readl(reg_addr+RK_GPIO_WRITE_REG)|(1ul<<index), reg_addr+RK_GPIO_WRITE_REG);
    else gpio_writel(gpio_readl(reg_addr+RK_GPIO_WRITE_REG)&(~(1ul<<index)), reg_addr+RK_GPIO_WRITE_REG);
    return 0;
}

int gpio_get_value(unsigned gpio)
{
	int reg_addr = gpio&0xffff0000;
    int index = gpio&0xffff;
    if(reg_addr==0)
    {
        gpio_err("no gpio group \n");
    }
    return (gpio_readl(reg_addr+RK_GPIO_READ_REG) >> index)&0x1;
}

int gpio_request(unsigned gpio, const char *label)
{
	return 0;
}

int gpio_free(unsigned gpio)
{
	return 0;
}

int gpio_irq_state(unsigned gpio)
{
    int reg_addr = gpio&0xffff0000;
    int index = gpio&0xffff;
    if(reg_addr==0)
    {
        gpio_err("no gpio group \n");
    }
    return (gpio_readl(reg_addr+RK_GPIO_INT_STATUS) >> index)&0x1;
}

int gpio_irq_clr(unsigned gpio)
{
    int reg_addr = gpio&0xffff0000;
    int index = gpio&0xffff;
    if(reg_addr==0)
    {
        gpio_err("no gpio group \n");
        return -1;
    }
    gpio_writel(gpio_readl(reg_addr+RK_GPIO_INT_EOI)|(1ul<<index), reg_addr+RK_GPIO_INT_EOI);

    return 0;
}

int gpio_irq_request(unsigned gpio, int type)
{
    int reg_addr = gpio&0xffff0000;
    int index = gpio&0xffff;
    if(reg_addr==0)
    {
        gpio_err("no gpio group 0x%x\n",gpio);
        return -1;
    }
    //gpio_err("%s,reg_addr=0x%x,index=%d,type=%d,addr = %x\n",__func__,reg_addr,index,type,reg_addr+RK_GPIO_DEBOUNCE_REG);
    gpio_writel(gpio_readl(reg_addr+RK_GPIO_DIR_REG)&(~(1ul<<index)), reg_addr+RK_GPIO_DIR_REG);   //input
	gpio_writel(gpio_readl(reg_addr+RK_GPIO_INT_MASK)&(~(1ul<<index)), reg_addr+RK_GPIO_INT_MASK); //int mask
    gpio_writel(gpio_readl(reg_addr+RK_GPIO_DEBOUNCE_REG)|(1ul<<index), reg_addr+RK_GPIO_DEBOUNCE_REG);  //debounce
 
    if((type & IRQ_TYPE_EDGE_RISING) || (type&IRQ_TYPE_EDGE_FALLING))
        gpio_writel(gpio_readl(reg_addr+RK_GPIO_INT_LEVEL)|(1ul<<index), reg_addr+RK_GPIO_INT_LEVEL);   //use edge sensitive
    else if((type & IRQ_TYPE_LEVEL_HIGH) || (type&IRQ_TYPE_LEVEL_LOW))
        gpio_writel(gpio_readl(reg_addr+RK_GPIO_INT_LEVEL)&(~(1ul<<index)), reg_addr+RK_GPIO_INT_LEVEL); //use level 

    if((type & IRQ_TYPE_EDGE_RISING) || (type&IRQ_TYPE_LEVEL_HIGH))
        gpio_writel(gpio_readl(reg_addr+RK_GPIO_INT_POLARITY)|(1ul<<index), reg_addr+RK_GPIO_INT_POLARITY);		
	else if((type & IRQ_TYPE_EDGE_FALLING) || (type&IRQ_TYPE_LEVEL_LOW))
		gpio_writel(gpio_readl(reg_addr+RK_GPIO_INT_POLARITY)&(~(1ul<<index)), reg_addr+RK_GPIO_INT_POLARITY);

    if(type)
	    gpio_writel(gpio_readl(reg_addr+RK_GPIO_INT_EN)|(1ul<<index), reg_addr+RK_GPIO_INT_EN);          //enable int
    else gpio_writel(gpio_readl(reg_addr+RK_GPIO_INT_EN)&(~(1ul<<index)), reg_addr+RK_GPIO_INT_EN);
    return 0;
}

int gpio_direction_input(unsigned gpio)
{
	int reg_addr = gpio&0xffff0000;
    int index = gpio&0xffff;
    if(reg_addr==0)
    {
        gpio_err("no gpio group 0x%x\n",gpio);
        return -1;
    }
    gpio_writel(gpio_readl(reg_addr+RK_GPIO_DIR_REG)&(~(1ul<<index)), reg_addr+RK_GPIO_DIR_REG);
   // gpio_writel(reg_addr+RK_GPIO_DEBOUNCE_REG,  gpio_readl(reg_addr+RK_GPIO_DEBOUNCE_REG)|(1ul<<index));
    return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
    int reg_addr = gpio&0xffff0000;
    int index = gpio&0xffff;
    if(reg_addr==0)
    {
        gpio_err("no gpio group 0x%x\n",gpio);
        return -1;
    }
    gpio_writel(gpio_readl(reg_addr+RK_GPIO_DIR_REG)|(1ul<<index), reg_addr+RK_GPIO_DIR_REG);
	return gpio_set_value(gpio, value);
}
