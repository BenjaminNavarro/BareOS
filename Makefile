# Copyright 2015 Philipp Oppermann. See the README.md
# file at the top-level directory of this distribution.
#
# Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
# http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
# <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
# option. This file may not be copied, modified, or distributed
# except according to those terms.

arch ?= x86_64
kernel := build/kernel-$(arch).bin
main := build/main/libmain.a
iso := build/os-$(arch).iso
libc := newlib-2.4.0/newlib/libc.a

ifeq ($(shell uname -s),Darwin)
	gcc_prefix := 'x86_64-elf-'
else
	gcc_prefix := ''
endif

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(arch)/*.asm)
assembly_object_files := $(patsubst src/arch/$(arch)/%.asm, \
	build/arch/$(arch)/%.o, $(assembly_source_files))

main_source_files := $(wildcard src/main/*.c)
main_object_files := $(patsubst src/main/%.c, \
	build/main/%.o, $(main_source_files))

.PHONY: all clean run iso main

all: $(main) $(kernel)

clean:
	@rm -r build

run: $(iso)
	@qemu-system-x86_64 -cdrom $(iso)

iso: $(iso)

$(iso): $(main) $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	@rm -r build/isofiles

$(kernel): $(assembly_object_files) $(linker_script) $(main)
	@$(gcc_prefix)ld -n -T $(linker_script) -o $(kernel) $(assembly_object_files) $(main_object_files) $(libc)

# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.asm
	@mkdir -p $(shell dirname $@)
	@nasm -felf64 $< -o $@

main: $(main)

$(main) : $(main_object_files)

build/main/%.o: src/main/%.c
	@mkdir -p $(shell dirname $@)
	@$(gcc_prefix)gcc -g -c $< -o $@
	@$(gcc_prefix)ar rcs $(main) $@
