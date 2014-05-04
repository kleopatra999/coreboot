

bootblock-y += cache.c
bootblock-y += cbmem.c
bootblock-$(CONFIG_BOOTBLOCK_CONSOLE) += uart.c
bootblock-y += timer.c

romstage-y += cbmem.c
romstage-y += cache.c
romstage-$(CONFIG_EARLY_CONSOLE) += uart.c
romstage-y += timer.c
romstage-y += i2c.c
romstage-y += clock.c

ramstage-y += cpu.c
ramstage-y += cache.c
ramstage-y += cbmem.c
ramstage-y += uart.c
ramstage-y += timer.c
ramstage-y += i2c.c
