# R4R License: MIT
#
# Common Makefile fragment for kernels submodules (core, devs, ...)
#
# (C) Copyright 2025 Isa <isa@isoux.org>

include ../../defines.mk

CFLAGS = -std=c23 -m32 -ffreestanding -Wall -Wextra -march=$(ARCH) -c -Wall \
    -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-pie \
    -fno-stack-protector \
    -I $(INCLUDE_DIR) -fpack-struct -DMARCH=$(ARCH)

CFLAGS += -O1 -fno-pic -fno-pie -mno-red-zone

SRC := $(wildcard *.c)
BASE := $(basename $(notdir $(SRC)))
OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(BASE)))
DUMP := $(addprefix $(DUMP_SUBDIR)/, $(addsuffix .dump, $(BASE)))

include $(wildcard */Makefile)

.PHONY: all clean dep

ifeq ($(OBJ_DUMP),"TRUE")
all: $(OBJ) $(DUMP)
else
all: $(OBJ)
endif

# Compile .c -> .o
$(OBJ_DIR)/%.o: %.c
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Generate .dump -> from .o
$(DUMP_SUBDIR)/%.dump: $(OBJ_DIR)/%.o
	$(MKDIR) $(DUMP_SUBDIR)
	$(OBJDUMP) $< > $@

clean:
	$(RM) $(OBJ) $(DUMP_FILE)

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c; do $(CPP) -M $$i; done) >> tmp_make
	cp tmp_make Makefile
