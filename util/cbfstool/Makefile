obj ?= $(shell pwd)

HOSTCC ?= gcc
CFLAGS ?= -g
CFLAGS += -D_7ZIP_ST
CFLAGS += -Wall -Wundef -Wstrict-prototypes -Wmissing-prototypes
CFLAGS += -Wwrite-strings -Wredundant-decls -Wno-trigraphs
CFLAGS += -Wstrict-aliasing -Wshadow -Werror
LDFLAGS += -g

BINARY:=$(obj)/cbfstool

COMMON:=cbfstool.o common.o cbfs_image.o compress.o fit.o
COMMON+=elfheaders.o cbfs-mkstage.o cbfs-mkpayload.o xdr.o
# LZMA
COMMON+=lzma/lzma.o
COMMON+=lzma/C/LzFind.o  lzma/C/LzmaDec.o  lzma/C/LzmaEnc.o

COMMON:=$(addprefix $(obj)/,$(COMMON))

all: dep $(BINARY)

$(obj)/%.o: %.c
	$(HOSTCC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(COMMON) $(BINARY)

tags:
	ctags *.[ch]

$(obj)/cbfstool:$(COMMON)
	$(HOSTCC) $(CFLAGS) -o $@ $^

dep:
	@$(HOSTCC) $(CFLAGS) -MM *.c > .dependencies
	@$(HOSTCC) $(CFLAGS) -MM lzma/*.c >> .dependencies
	@$(HOSTCC) $(CFLAGS) -MM lzma/C/*.c >> .dependencies

-include .dependencies
