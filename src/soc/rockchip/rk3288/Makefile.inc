IDBTOOL = makeIDBTool

bootblock-y += bootblock.c
bootblock-y += cache.c
bootblock-y += cbmem.c
bootblock-$(CONFIG_BOOTBLOCK_CONSOLE) += uart.c
bootblock-y += timer.c
bootblock-y += monotonic_timer.c
bootblock-y += pinmux.c
bootblock-y += spi.c

romstage-y += cbmem.c
romstage-y += cache.c
romstage-$(CONFIG_EARLY_CONSOLE) += uart.c
romstage-y += timer.c
romstage-y += monotonic_timer.c
romstage-y += i2c.c
romstage-y += clock.c
romstage-y += gpio.c
romstage-y += pinmux.c
romstage-y += spi.c
romstage-y += ddr.c
romstage-y += dramMain.c
romstage-y += dramCTL.c

romstage-y += noc.c

ramstage-y += cpu.c
ramstage-y += cache.c
ramstage-y += cbmem.c
ramstage-y += uart.c
ramstage-y += timer.c
ramstage-y += monotonic_timer.c
ramstage-y += i2c.c
ramstage-y += pinmux.c
ramstage-y += spi.c
ramstage-y += ddr.c
ramstage-y += dramMain.c
ramstage-y += dramCTL.c
ramstage-y += noc.c

$(objcbfs)/bootblock.raw.elf: $(objcbfs)/bootblock.elf
	cp $< $@

IDB_BIN = $(obj)/generated/IDBlock.bin

# default output is ./IDBlock.bin
$(IDB_BIN): $(objcbfs)/bootblock.raw.bin
	@printf "Generating: $(subst $(obj)/,,$(@))\n"
	@$(IDBTOOL) $< 0 1
	@mkdir -p $(dir $@)
	@mv ./IDBlock.bin $@

# copy 5 times.
$(objcbfs)/bootblock.bin: $(IDB_BIN)
	@printf "Generating: $(subst $(obj)/,,$(@))\n"
	@echo -n > $@
	@for i in $$(seq 1); do \
		cat $< >> $@ ; \
	done
	