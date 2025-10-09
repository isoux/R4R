#
# R4R License: MIT
#
# Makefile for kernels/core/print
#
# (C) Copyright 2025 Isa <isa@isoux.org>

#$(info === OBJDIR $(OBJ_DIR) module $(MODULE))

# Objects from core/print
OBJ += $(OBJ_DIR)/print/core_print.o
OBJ += $(OBJ_DIR)/print/core_textio.o

DUMP += $(DUMP_SUBDIR)/print/core_print.dump
DUMP += $(DUMP_SUBDIR)/print/core_textio.dump

# Rule for compiling sources inside core/print
$(OBJ_DIR)/print/%.o: print/%.c
	$(MKDIR) $(OBJ_DIR)/print
	$(CC) $(CFLAGS) -o $@ $<
	
# Generate .dump -> from .o
$(DUMP_SUBDIR)/print/%.dump: $(OBJ_DIR)/print/%.o
	$(MKDIR) $(DUMP_SUBDIR)/print
	$(OBJDUMP) $< > $@

$(info === loading core/print/Makefile)