################################################################################
##
## This file is part of the coreboot project.
##
## Copyright (C) 2012 The ChromiumOS Authors
### Copyright (C) 2012 Alexandru Gagniuc <mr.nuke.me@gmail.com>
## Copyright (C) 2009-2010 coresystems GmbH
## Copyright (C) 2009 Ronald G. Minnich
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
##
################################################################################

################################################################################
# Build the final rom image
COREBOOT_ROM_DEPENDENCIES:=
ifeq ($(CONFIG_PAYLOAD_ELF),y)
COREBOOT_ROM_DEPENDENCIES+=$(CONFIG_PAYLOAD_FILE)
endif

extract_nth=$(word $(1), $(subst |, ,$(2)))

ifneq ($(CONFIG_UPDATE_IMAGE),y)
prebuild-files = \
	$(foreach file,$(cbfs-files), \
	$(CBFSTOOL) $@.tmp \
	add$(if $(filter stage,$(call extract_nth,3,$(file))),-stage)$(if $(filter payload,$(call extract_nth,3,$(file))),-payload) \
	-f $(call extract_nth,1,$(file)) \
	-n $(call extract_nth,2,$(file)) $(if $(filter-out stage,$(call extract_nth,3,$(file))),-t $(call extract_nth,3,$(file))) \
	$(if $(call extract_nth,4,$(file)),-b $(call extract_nth,4,$(file))) &&)
prebuilt-files = $(foreach file,$(cbfs-files), $(call extract_nth,1,$(file)))

# TODO Change -b to Kconfig variable.
$(obj)/coreboot.pre: $(objcbfs)/bootblock.bin $(objcbfs)/romstage.elf $$(prebuilt-files) $(CBFSTOOL) $$(cpu_ucode_cbfs_file)
	$(CBFSTOOL) $@.tmp create -m aarch64 -s $(CONFIG_COREBOOT_ROMSIZE_KB)K \
		-B $(objcbfs)/bootblock.bin -a 64 \
		-b $(CONFIG_BOOTBLOCK_ROM_OFFSET) \
		-H $(CONFIG_CBFS_HEADER_ROM_OFFSET) \
		-o $(CONFIG_CBFS_ROM_OFFSET)
	@printf "    CBFS       $(subst $(obj)/,,$(@))\n"
	$(CBFSTOOL) $@.tmp add-stage \
		-f $(objcbfs)/romstage.elf -b 0 \
		-n $(CONFIG_CBFS_PREFIX)/romstage -c none
	$(prebuild-files) true
	$(call add-cpu-microcode-to-cbfs,$@.tmp)
	mv $@.tmp $@
else
.PHONY: $(obj)/coreboot.pre
$(obj)/coreboot.pre: $(CBFSTOOL)
	mv $(obj)/coreboot.rom $@
endif

$(obj)/coreboot.rom: $(obj)/coreboot.pre $(objcbfs)/coreboot_ram.elf $(CBFSTOOL) $(call strip_quotes,$(COREBOOT_ROM_DEPENDENCIES)) $$(INTERMEDIATE)
	@printf "    CBFS       $(subst $(obj)/,,$(@))\n"
	cp $(obj)/coreboot.pre $@.tmp
	$(CBFSTOOL) $@.tmp add-stage -f $(objcbfs)/coreboot_ram.elf -n $(CONFIG_CBFS_PREFIX)/coreboot_ram -c $(CBFS_COMPRESS_FLAG)
ifeq ($(CONFIG_PAYLOAD_NONE),y)
	@printf "    PAYLOAD    none (as specified by user)\n"
endif
ifeq ($(CONFIG_PAYLOAD_ELF),y)
	@printf "    PAYLOAD    $(CONFIG_PAYLOAD_FILE) (compression: $(CBFS_PAYLOAD_COMPRESS_NAME))\n"
	$(CBFSTOOL) $@.tmp add-payload -f $(CONFIG_PAYLOAD_FILE) -n $(CONFIG_CBFS_PREFIX)/payload -c $(CBFS_PAYLOAD_COMPRESS_FLAG)
endif
ifeq ($(CONFIG_INCLUDE_CONFIG_FILE),y)
	@printf "    CONFIG     $(DOTCONFIG)\n"
	if [ -f $(DOTCONFIG) ]; then \
	echo "# This image was built using git revision" `git rev-parse HEAD` > $(obj)/config.tmp ; \
	sed -e '/^#/d' -e '/^ *$$/d' $(DOTCONFIG) >> $(obj)/config.tmp ; \
	$(CBFSTOOL) $@.tmp add -f $(obj)/config.tmp -n config -t raw; rm -f $(obj)/config.tmp ; fi
endif
	mv $@.tmp $@
	@printf "    CBFSPRINT  $(subst $(obj)/,,$(@))\n\n"
	$(CBFSTOOL) $@ print

bootsplash.jpg-file := $(call strip_quotes,$(CONFIG_BOOTSPLASH_FILE))
bootsplash.jpg-type := bootsplash

################################################################################
# aarch64 specific tools

################################################################################
# Common recipes for all stages

$(objcbfs)/%.bin: $(objcbfs)/%.elf
	@printf "    OBJCOPY    $(subst $(obj)/,,$(@))\n"
	$(OBJCOPY) -O binary $< $@

$(objcbfs)/%.elf: $(objcbfs)/%.debug
	@printf "    OBJCOPY    $(subst $(obj)/,,$(@))\n"
	cp $< $@.tmp
	$(NM) -n $@.tmp | sort > $(basename $@).map
	$(OBJCOPY) --strip-debug $@.tmp
	$(OBJCOPY) --add-gnu-debuglink=$< $@.tmp
	mv $@.tmp $@

################################################################################
# Build the coreboot_ram (stage 2)

$(objcbfs)/coreboot_ram.debug: $(objgenerated)/coreboot_ram.o $(src)/arch/aarch64/coreboot_ram.ld
	@printf "    CC         $(subst $(obj)/,,$(@))\n"
ifeq ($(CONFIG_COMPILER_LLVM_CLANG),y)
	$(LD) -m armelf_linux_eabi -o $@ -L$(obj) $< -T $(src)/arch/aarch64/coreboot_ram.ld
else
	$(CC) -nostdlib -nostartfiles -static -o $@ -L$(obj) -T $(src)/arch/aarch64/coreboot_ram.ld $<
endif

$(objgenerated)/coreboot_ram.o: $(stages_o) $$(ramstage-objs) $(LIBGCC_FILE_NAME)
	@printf "    CC         $(subst $(obj)/,,$(@))\n"
ifeq ($(CONFIG_COMPILER_LLVM_CLANG),y)
	$(LD) -m -m armelf_linux_eabi -r -o $@ --wrap __divdi3 --wrap __udivdi3 --wrap __moddi3 --wrap __umoddi3 --wrap __uidiv --start-group $(ramstage-objs) $(LIBGCC_FILE_NAME) --end-group
else
	$(CC) $(CFLAGS) -nostdlib -r -o $@ -Wl,--start-group $(stages_o) $(ramstage-objs) $(LIBGCC_FILE_NAME) -Wl,--end-group
endif

CFLAGS += -mstrict-align \

ldscripts =
ldscripts += $(src)/arch/aarch64/romstage.ld

#crt0s += $(src)/cpu/arm/fpu_enable.inc

crt0s += $(cpu_incs)
crt0s += $(cpu_incs-y)

$(obj)/mainboard/$(MAINBOARDDIR)/romstage.pre.inc: $(src)/mainboard/$(MAINBOARDDIR)/romstage.c $(OPTION_TABLE_H) $(obj)/build.h $(obj)/config.h
	@printf "    CC         romstage.inc\n"
	$(CC) -MMD $(CFLAGS) -D__PRE_RAM__ -I$(src) -I. -I$(obj) -c -S $< -o $@

# Things that appear in every board
ramstage-y += exception.c
ramstage-y += exception_asm.S

bootblock-$(CONFIG_BOOTBLOCK_CONSOLE) += early_console.c
bootblock-y += cache.c
bootblock-y += cpu.S
bootblock-$(CONFIG_BOOTBLOCK_CONSOLE) += exception.c
bootblock-$(CONFIG_BOOTBLOCK_CONSOLE) += exception_asm.S

romstage-y += cache.c
romstage-y += cpu.S
#romstage-y += div0.c
romstage-y += exception.c
romstage-y += exception_asm.S
romstage-$(CONFIG_EARLY_CONSOLE) += early_console.c
romstage-y += cbmem.c

bootblock-y += stages.c
romstage-y += stages.c
ramstage-y += stages.c

#ramstage-y += div0.c
#ramstage-y += interrupts.c
ramstage-y += cache.c
ramstage-y += cpu.S
#ramstage-y += mmu.c

#romstage-y += eabi_compat.c
#ramstage-y += eabi_compat.c
#bootblock-y += eabi_compat.c

ramstage-y += boot.c
ramstage-y += tables.c
ramstage-y += cbmem.c

#romstage-y += memset.S
#ramstage-y += memset.S
#bootblock-y += memset.S
#romstage-y += memcpy.S
#ramstage-y += memcpy.S
#bootblock-y += memcpy.S
#romstage-y += memmove.S
#ramstage-y += memmove.S
#bootblock-y += memmove.S

$(obj)/arch/aarch64/coreboot_table.ramstage.o : $(OPTION_TABLE_H)

romstage-srcs += $(objgenerated)/crt0.s
ramstage-srcs += $(wildcard src/mainboard/$(MAINBOARDDIR)/mainboard.c)

ifeq ($(CONFIG_BOARD_HAS_HARD_RESET),y)
#ramstage-srcs += src/mainboard/$(MAINBOARDDIR)/reset.c
endif

ifeq ($(CONFIG_HAVE_BUS_CONFIG),y)
#ramstage-srcs += src/mainboard/$(MAINBOARDDIR)/get_bus_conf.c
endif

################################################################################
# Build the bootblock

bootblock_lds = $(src)/arch/aarch64/bootblock.lds
bootblock_lds += $(chipset_bootblock_lds)

bootblock_inc += $(src)/arch/aarch64/bootblock.inc
bootblock_inc += $(chipset_bootblock_inc)
bootblock_inc += $(objgenerated)/bootblock.inc

bootblock_custom = $(src)/$(call strip_quotes,$(CONFIG_BOOTBLOCK_CPU_INIT))
bootblock_custom += $(src)/$(call strip_quotes,$(CONFIG_BOOTBLOCK_MAINBOARD_INIT))

$(objgenerated)/bootblock.ld: $$(bootblock_lds) $(obj)/ldoptions
	@printf "    GEN        $(subst $(obj)/,,$(@))\n"
	printf '$(foreach ldscript,ldoptions $(bootblock_lds),INCLUDE "$(ldscript)"\n)' > $@

$(objgenerated)/bootblock_inc.S: $$(bootblock_inc)
	@printf "    GEN        $(subst $(obj)/,,$(@))\n"
	printf '$(foreach crt0,$(bootblock_inc),#include "$(crt0)"\n)' > $@

$(objgenerated)/bootblock.o: $(objgenerated)/bootblock.s
	@printf "    CC         $(subst $(obj)/,,$(@))\n"
	$(CC) $(bootblock-S-ccopts) -Wa,-acdlns -c -o $@ $<  > $(basename $@).disasm

$(objgenerated)/bootblock.s: $(objgenerated)/bootblock_inc.S $(obj)/config.h $(obj)/build.h
	@printf "    CC         $(subst $(obj)/,,$(@))\n"
	$(CC) $(bootblock-S-ccopts) -MMD -x assembler-with-cpp -E -I$(src)/include -I$(src)/arch/aarch64/include -I$(obj) -include $(obj)/build.h -include $(obj)/config.h -I. -I$(src) $< -o $@

$(objgenerated)/bootblock.inc: $(src)/arch/aarch64/$(subst ",,$(CONFIG_BOOTBLOCK_SOURCE)) $(bootblock_custom) $(OPTION_TABLE_H) $(obj)/config.h
	@printf "    CC      $(subst $(obj)/,,$(@))\n"
	$(CC) $(bootblock-c-ccopts) $(INCLUDES) -MM \
		-MT$(objgenerated)/bootblock.inc \
		$< > $(objgenerated)/bootblock.inc.d
	$(CC) $(bootblock-c-ccopts) -c -S $(CFLAGS) -I. $(INCLUDES) $< -o $@

$(objcbfs)/bootblock.debug:  $(objgenerated)/bootblock.o $(objgenerated)/bootblock.ld $$(bootblock-objs) $(stages) $(obj)/config.h
	@printf "    LINK       $(subst $(obj)/,,$(@))\n"
ifeq ($(CONFIG_COMPILER_LLVM_CLANG),y)
	$(LD) -m armelf_linux_eabi -include $(obj)/config.h -static -o $@.tmp -L$(obj) $< -T $(objgenerated)/bootblock.ld
else
	$(CC) -nostdlib -nostartfiles -include $(obj)/config.h -static -o $@ -L$(obj) -T $(objgenerated)/bootblock.ld -Wl,--start-group $(objgenerated)/bootblock.o $(bootblock-objs) $(stages) $(LIBGCC_FILE_NAME) -Wl,--end-group
endif

################################################################################
# Build the romstage

$(objcbfs)/romstage.debug: $$(romstage-objs) $(stages_o) $(objgenerated)/romstage.ld
	@printf "    LINK       $(subst $(obj)/,,$(@))\n"
ifeq ($(CONFIG_COMPILER_LLVM_CLANG),y)
	$(LD) -nostdlib -nostartfiles -static -o $@ -L$(obj) $(romstage-objs) -T $(objgenerated)/romstage.ld
else
	$(CC) -nostdlib -nostartfiles -static -o $@ -L$(obj) -T $(objgenerated)/romstage.ld -Wl,--start-group $(romstage-objs) $(stages_o) $(LIBGCC_FILE_NAME) -Wl,--end-group
endif

$(objgenerated)/romstage.ld: $$(ldscripts) $(obj)/ldoptions
	@printf "    GEN        $(subst $(obj)/,,$(@))\n"
	rm -f $@
	printf '$(foreach ldscript,ldoptions $(ldscripts),INCLUDE "$(ldscript:$(obj)/%=%)"\n)' >> $@.tmp
	mv $@.tmp $@

$(objgenerated)/crt0.romstage.S: $$(crt0s)
	@printf "    GEN        $(subst $(obj)/,,$(@))\n"
	printf '$(foreach crt0,$(crt0s),#include "$(crt0:$(obj)/%=%)"\n)' > $@

$(objgenerated)/crt0.romstage.o: $(objgenerated)/crt0.s
	@printf "    CC         $(subst $(obj)/,,$(@))\n"
	$(CC) -Wa,-acdlns -c -o $@ $<  > $(basename $@).disasm

$(objgenerated)/crt0.s: $(objgenerated)/crt0.romstage.S $(obj)/config.h $(obj)/build.h
	@printf "    CC         $(subst $(obj)/,,$(@))\n"
	$(CC) -MMD -x assembler-with-cpp -E -I$(src)/include -I$(src)/arch/aarch64/include -I$(obj) -include $(obj)/config.h -include $(obj)/build.h -I. -I$(src) $< -o $@

