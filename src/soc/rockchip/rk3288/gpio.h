/*
 * rockchip gpio driver
 */

#ifndef _ASM_ROCKCHIP_GPIO_H_
#define _ASM_ROCKCHIP_GPIO_H_

#include "reg.h"
#include "memap.h"

#define RK_GPIO_WRITE_REG        0
#define RK_GPIO_DIR_REG          0x4
#define RK_GPIO_INT_EN           0x30
#define RK_GPIO_INT_MASK         0x34
#define RK_GPIO_INT_LEVEL        0x38
#define RK_GPIO_INT_POLARITY     0x3c
#define RK_GPIO_INT_STATUS       0x40
#define RK_GPIO_DEBOUNCE_REG     0x48
#define RK_GPIO_INT_EOI          0x4c
#define RK_GPIO_READ_REG         0x50

enum rk32_gpio_pin {
    RK_GPIO0_A0 = GPIO0_BASE_ADDR,
    RK_GPIO0_A1,
    RK_GPIO0_A2,
    RK_GPIO0_A3,
    RK_GPIO0_A4,
    RK_GPIO0_A5,
    RK_GPIO0_A6,
    RK_GPIO0_A7,
    RK_GPIO0_B0,
    RK_GPIO0_B1,
    RK_GPIO0_B2,
    RK_GPIO0_B3,
    RK_GPIO0_B4,
    RK_GPIO0_B5,
    RK_GPIO0_B6,
    RK_GPIO0_B7,    
    RK_GPIO0_C0,
    RK_GPIO0_C1,
    RK_GPIO0_C2,
    RK_GPIO0_C3,
    RK_GPIO0_C4,
    RK_GPIO0_C5,
    RK_GPIO0_C6,
    RK_GPIO0_C7,
    RK_GPIO0_D0,
    RK_GPIO0_D1,
    RK_GPIO0_D2,
    RK_GPIO0_D3,
    RK_GPIO0_D4,
    RK_GPIO0_D5,
    RK_GPIO0_D6,
    RK_GPIO0_D7,

    RK_GPIO1_A0 = GPIO1_BASE_ADDR,
    RK_GPIO1_A1,
    RK_GPIO1_A2,
    RK_GPIO1_A3,
    RK_GPIO1_A4,
    RK_GPIO1_A5,
    RK_GPIO1_A6,
    RK_GPIO1_A7,
    RK_GPIO1_B0,
    RK_GPIO1_B1,
    RK_GPIO1_B2,
    RK_GPIO1_B3,
    RK_GPIO1_B4,
    RK_GPIO1_B5,
    RK_GPIO1_B6,
    RK_GPIO1_B7,    
    RK_GPIO1_C0,
    RK_GPIO1_C1,
    RK_GPIO1_C2,
    RK_GPIO1_C3,
    RK_GPIO1_C4,
    RK_GPIO1_C5,
    RK_GPIO1_C6,
    RK_GPIO1_C7,
    RK_GPIO1_D0,
    RK_GPIO1_D1,
    RK_GPIO1_D2,
    RK_GPIO1_D3,
    RK_GPIO1_D4,
    RK_GPIO1_D5,
    RK_GPIO1_D6,
    RK_GPIO1_D7, 

    RK_GPIO2_A0 = GPIO2_BASE_ADDR,
    RK_GPIO2_A1,
    RK_GPIO2_A2,
    RK_GPIO2_A3,
    RK_GPIO2_A4,
    RK_GPIO2_A5,
    RK_GPIO2_A6,
    RK_GPIO2_A7,
    RK_GPIO2_B0,
    RK_GPIO2_B1,
    RK_GPIO2_B2,
    RK_GPIO2_B3,
    RK_GPIO2_B4,
    RK_GPIO2_B5,
    RK_GPIO2_B6,
    RK_GPIO2_B7,    
    RK_GPIO2_C0,
    RK_GPIO2_C1,
    RK_GPIO2_C2,
    RK_GPIO2_C3,
    RK_GPIO2_C4,
    RK_GPIO2_C5,
    RK_GPIO2_C6,
    RK_GPIO2_C7,
    RK_GPIO2_D0,
    RK_GPIO2_D1,
    RK_GPIO2_D2,
    RK_GPIO2_D3,
    RK_GPIO2_D4,
    RK_GPIO2_D5,
    RK_GPIO2_D6,
    RK_GPIO2_D7,

    RK_GPIO3_A0 = GPIO3_BASE_ADDR,
    RK_GPIO3_A1,
    RK_GPIO3_A2,
    RK_GPIO3_A3,
    RK_GPIO3_A4,
    RK_GPIO3_A5,
    RK_GPIO3_A6,
    RK_GPIO3_A7,
    RK_GPIO3_B0,
    RK_GPIO3_B1,
    RK_GPIO3_B2,
    RK_GPIO3_B3,
    RK_GPIO3_B4,
    RK_GPIO3_B5,
    RK_GPIO3_B6,
    RK_GPIO3_B7,    
    RK_GPIO3_C0,
    RK_GPIO3_C1,
    RK_GPIO3_C2,
    RK_GPIO3_C3,
    RK_GPIO3_C4,
    RK_GPIO3_C5,
    RK_GPIO3_C6,
    RK_GPIO3_C7,
    RK_GPIO3_D0,
    RK_GPIO3_D1,
    RK_GPIO3_D2,
    RK_GPIO3_D3,
    RK_GPIO3_D4,
    RK_GPIO3_D5,
    RK_GPIO3_D6,
    RK_GPIO3_D7,  

    RK_GPIO4_A0 = GPIO4_BASE_ADDR,
    RK_GPIO4_A1,
    RK_GPIO4_A2,
    RK_GPIO4_A3,
    RK_GPIO4_A4,
    RK_GPIO4_A5,
    RK_GPIO4_A6,
    RK_GPIO4_A7,
    RK_GPIO4_B0,
    RK_GPIO4_B1,
    RK_GPIO4_B2,
    RK_GPIO4_B3,
    RK_GPIO4_B4,
    RK_GPIO4_B5,
    RK_GPIO4_B6,
    RK_GPIO4_B7,    
    RK_GPIO4_C0,
    RK_GPIO4_C1,
    RK_GPIO4_C2,
    RK_GPIO4_C3,
    RK_GPIO4_C4,
    RK_GPIO4_C5,
    RK_GPIO4_C6,
    RK_GPIO4_C7,
    RK_GPIO4_D0,
    RK_GPIO4_D1,
    RK_GPIO4_D2,
    RK_GPIO4_D3,
    RK_GPIO4_D4,
    RK_GPIO4_D5,
    RK_GPIO4_D6,
    RK_GPIO4_D7,  

    RK_GPIO5_A0 = GPIO5_BASE_ADDR,
    RK_GPIO5_A1,
    RK_GPIO5_A2,
    RK_GPIO5_A3,
    RK_GPIO5_A4,
    RK_GPIO5_A5,
    RK_GPIO5_A6,
    RK_GPIO5_A7,
    RK_GPIO5_B0,
    RK_GPIO5_B1,
    RK_GPIO5_B2,
    RK_GPIO5_B3,
    RK_GPIO5_B4,
    RK_GPIO5_B5,
    RK_GPIO5_B6,
    RK_GPIO5_B7,    
    RK_GPIO5_C0,
    RK_GPIO5_C1,
    RK_GPIO5_C2,
    RK_GPIO5_C3,
    RK_GPIO5_C4,
    RK_GPIO5_C5,
    RK_GPIO5_C6,
    RK_GPIO5_C7,
    RK_GPIO5_D0,
    RK_GPIO5_D1,
    RK_GPIO5_D2,
    RK_GPIO5_D3,
    RK_GPIO5_D4,
    RK_GPIO5_D5,
    RK_GPIO5_D6,
    RK_GPIO5_D7,  

    RK_GPIO6_A0 = GPIO6_BASE_ADDR,
    RK_GPIO6_A1,
    RK_GPIO6_A2,
    RK_GPIO6_A3,
    RK_GPIO6_A4,
    RK_GPIO6_A5,
    RK_GPIO6_A6,
    RK_GPIO6_A7,
    RK_GPIO6_B0,
    RK_GPIO6_B1,
    RK_GPIO6_B2,
    RK_GPIO6_B3,
    RK_GPIO6_B4,
    RK_GPIO6_B5,
    RK_GPIO6_B6,
    RK_GPIO6_B7,    
    RK_GPIO6_C0,
    RK_GPIO6_C1,
    RK_GPIO6_C2,
    RK_GPIO6_C3,
    RK_GPIO6_C4,
    RK_GPIO6_C5,
    RK_GPIO6_C6,
    RK_GPIO6_C7,
    RK_GPIO6_D0,
    RK_GPIO6_D1,
    RK_GPIO6_D2,
    RK_GPIO6_D3,
    RK_GPIO6_D4,
    RK_GPIO6_D5,
    RK_GPIO6_D6,
    RK_GPIO6_D7,  

    RK_GPIO7_A0 = GPIO7_BASE_ADDR,
    RK_GPIO7_A1,
    RK_GPIO7_A2,
    RK_GPIO7_A3,
    RK_GPIO7_A4,
    RK_GPIO7_A5,
    RK_GPIO7_A6,
    RK_GPIO7_A7,
    RK_GPIO7_B0,
    RK_GPIO7_B1,
    RK_GPIO7_B2,
    RK_GPIO7_B3,
    RK_GPIO7_B4,
    RK_GPIO7_B5,
    RK_GPIO7_B6,
    RK_GPIO7_B7,    
    RK_GPIO7_C0,
    RK_GPIO7_C1,
    RK_GPIO7_C2,
    RK_GPIO7_C3,
    RK_GPIO7_C4,
    RK_GPIO7_C5,
    RK_GPIO7_C6,
    RK_GPIO7_C7,
    RK_GPIO7_D0,
    RK_GPIO7_D1,
    RK_GPIO7_D2,
    RK_GPIO7_D3,
    RK_GPIO7_D4,
    RK_GPIO7_D5,
    RK_GPIO7_D6,
    RK_GPIO7_D7,  

    RK_GPIO8_A0 = GPIO8_BASE_ADDR,
    RK_GPIO8_A1,
    RK_GPIO8_A2,
    RK_GPIO8_A3,
    RK_GPIO8_A4,
    RK_GPIO8_A5,
    RK_GPIO8_A6,
    RK_GPIO8_A7,
    RK_GPIO8_B0,
    RK_GPIO8_B1,
    RK_GPIO8_B2,
    RK_GPIO8_B3,
    RK_GPIO8_B4,
    RK_GPIO8_B5,
    RK_GPIO8_B6,
    RK_GPIO8_B7,    
    RK_GPIO8_C0,
    RK_GPIO8_C1,
    RK_GPIO8_C2,
    RK_GPIO8_C3,
    RK_GPIO8_C4,
    RK_GPIO8_C5,
    RK_GPIO8_C6,
    RK_GPIO8_C7,
    RK_GPIO8_D0,
    RK_GPIO8_D1,
    RK_GPIO8_D2,
    RK_GPIO8_D3,
    RK_GPIO8_D4,
    RK_GPIO8_D5,
    RK_GPIO8_D6,
    RK_GPIO8_D7  
};


/*
 * This is Linux-specific flags. By default controllers' and Linux' mapping
 * match, but GPIO controllers are free to translate their own flags to
 * Linux-specific in their .xlate callback. Though, 1:1 mapping is recommended.
 */
enum of_gpio_flags {
	OF_GPIO_ACTIVE_LOW = 0x1,
};

enum {
	IRQ_TYPE_NONE		= 0x00000000,
	IRQ_TYPE_EDGE_RISING	= 0x00000001,
	IRQ_TYPE_EDGE_FALLING	= 0x00000002,
	IRQ_TYPE_EDGE_BOTH	= (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING),
	IRQ_TYPE_LEVEL_HIGH	= 0x00000004,
	IRQ_TYPE_LEVEL_LOW	= 0x00000008,
	IRQ_TYPE_LEVEL_MASK	= (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH),
	IRQ_TYPE_SENSE_MASK	= 0x0000000f,
	IRQ_TYPE_DEFAULT	= IRQ_TYPE_SENSE_MASK,
};

int gpio_request(unsigned gpio, const char *label);
int gpio_free(unsigned gpio);
int gpio_direction_input(unsigned gpio);
int gpio_direction_output(unsigned gpio, int value);
int gpio_get_value(unsigned gpio);
int gpio_set_value(unsigned gpio, int value);
int gpio_is_valid(int number);
int gpio_irq_request(unsigned gpio, int type);

#endif /* _ASM_ROCKCHIP_GPIO_H_ */
