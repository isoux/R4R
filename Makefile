# R4R License: MIT
#
# R4R/Makefile
#
# Main Makefile for full system build
#
# (C) Copyright 2025 Isa <isa@isoux.org>

SUBDIRS = src/sys/load src/sys/init src/sys/gdt src/sys/idt \
    src/sys/page src/kernels/core src/kernels/devs \
    src/kernels/libs src/kernels/users

.PHONY: all build clean link link-load link-init link-core \
	link-devs link-libs link-users sys

all: build link sys

build: 
	@echo "=== Compiling all components ==="
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@if [ -d build ]; then rm -rf build/*; fi

link:
	@echo "=== Linking all final binaries ==="
	$(MAKE) link-load
	$(MAKE) link-init
	$(MAKE) link-core
	$(MAKE) link-devs
	$(MAKE) link-libs
	$(MAKE) link-users

link-load:
	@echo "--- Linking load.bin ---"
	ld -T src/sys/load/load.ld -nostdlib  -m elf_i386 \
		build/load/load.o -o build/load/load.bin
	objdump -d -D -M intel build/load/load.bin >> build/dumps/load.dump

link-init:
	@echo "--- Linking init.elf ---"
	ld -T src/sys/init/init.ld -nostdlib  -m elf_i386 \
		build/init/init.o build/gdt/gdt_build.o build/gdt/gdt_init.o \
		build/idt/idt_build.o build/page/pages_build.o \
		build/init/main.o -o build/init/init.elf
	objdump -d -D -M intel build/init/init.elf >> build/dumps/init.dump
		
link-core: 
	@echo "--- Linking core.elf ---"
	ld -T src/kernels/core/core.ld -nostdlib  -m elf_i386 \
	    build/core/core_init.o build/core/core_task.o build/core/core_call_gates.o \
	    build/core/gdt.o build/core/idt_setup.o build/core/sys_exceptions.o \
	    build/core/core_syscalls.o build/core/sys_init.o build/core/print/core_print.o \
	    build/core/print/core_textio.o -o build/core/core.elf
	objdump -d -D -M intel build/core/core.elf >> build/dumps/core.dump
	
	
link-devs:
	@echo "--- Linking devs.elf ---"
	ld -T src/kernels/devs/devs.ld -nostdlib  -m elf_i386 \
	    build/devs/devs_init.o build/devs/devs_call_gates.o build/devs/devs_task.o \
	    build/devs/devs_irq.o build/devs/devs_sched.o \
	    -o build/devs/devs.elf
	objdump -d -D -M intel build/devs/devs.elf >> build/dumps/devs.dump
	
link-libs:
	@echo "--- Linking libs.elf ---"
	ld -T src/kernels/libs/libs.ld -nostdlib  -m elf_i386 \
	    build/libs/libs_init.o build/libs/libs_call_gates.o build/libs/libs_task.o \
	    build/libs/libs_irq.o build/libs/libs_sched.o \
	    -o build/libs/libs.elf
	objdump -d -D -M intel build/libs/libs.elf >> build/dumps/libs.dump
	
link-users:
	@echo "--- Linking libs.elf ---"
	ld -T src/kernels/users/users.ld -nostdlib  -m elf_i386 \
	    build/users/users_init.o build/users/users_task.o \
	    build/users/main_task.o build/users/nested_task.o \
	    -o build/users/users.elf
	objdump -d -D -M intel build/users/users.elf >> build/dumps/users.dump
	
sys:
	@echo "=== Preparing sys_out directory with load, init and grub.img ==="
	mkdir -pv build/sys_out
	cp build/load/load.bin build/sys_out/load
	objcopy -O binary -R .note -R .comment build/init/init.elf build/sys_out/init
	objcopy -O binary -R .note -R .comment build/core/core.elf build/sys_out/core
	objcopy -O binary -R .note -R .comment build/devs/devs.elf build/sys_out/devs
	objcopy -O binary -R .note -R .comment build/libs/libs.elf build/sys_out/libs
	objcopy -O binary -R .note -R .comment build/users/users.elf build/sys_out/users
	cp tools/GRUB1/grub1.img build/sys_out/grub1.img
	cp tools/GRUB1/bochsrc.txt build/sys_out/bochsrc.txt
	cp tools/GRUB1/bochs_from_make.txt build/sys_out/bochs_from_make.txt
	cp tools/GRUB1/menu.lst build/sys_out/menu.lst
	mcopy -i build/sys_out/grub1.img build/sys_out/menu.lst "::boot/grub/menu.lst"
	mcopy -i build/sys_out/grub1.img build/sys_out/load "::boot/load"
	mcopy -i build/sys_out/grub1.img build/sys_out/init "::boot/init"
	mcopy -i build/sys_out/grub1.img build/sys_out/core "::boot/core"
	mcopy -i build/sys_out/grub1.img build/sys_out/devs "::boot/devs"
	mcopy -i build/sys_out/grub1.img build/sys_out/libs "::boot/libs"
	mcopy -i build/sys_out/grub1.img build/sys_out/users "::boot/users"

bochs:
	bochs -q -f build/sys_out/bochs_from_make.txt
	