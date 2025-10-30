# ======================================
# fuckUNIX Makefile - modular build in build/
# ======================================

# Directories
BUILD_DIR := build

# Sources
C_SOURCES := $(wildcard kernel/*.c drivers/*.c cpu/*.c fuckFAT/*.c)
ASM_SOURCES := $(wildcard cpu/*.asm)
HEADERS := $(wildcard kernel/*.h drivers/*.h cpu/*.h fuckFAT/*.h)

# Object files in build/ directory
OBJ_FILES := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES)) \
             $(patsubst %.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

# Compiler and linker
CC ?= x86_64-elf-gcc
LD ?= x86_64-elf-ld

# ======================================
# Default rule
# ======================================
all: run-iso

# ======================================
# Pattern rules
# ======================================

# C -> .o
$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c $< -o $@

# ASM -> .o
$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	nasm $< -f elf -o $@

# ASM -> .bin (bootloader)
$(BUILD_DIR)/boot/mbr.bin: boot/mbr.asm
	@mkdir -p $(dir $@)
	nasm $< -f bin -o $@

# ======================================
# Kernel link
# ======================================
build/kernel/kernel.bin: $(BUILD_DIR)/boot/kernel_entry.o $(filter-out $(BUILD_DIR)/boot/kernel_entry.o,$(OBJ_FILES))
	@mkdir -p $(dir $@)
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# ======================================
# ISO creation
# ======================================
iso-image.iso: build/boot/mbr.bin build/kernel/kernel.bin
	mkdir -p iso/boot
	cp build/boot/mbr.bin iso/boot/
	cp build/kernel/kernel.bin iso/boot/
	genisoimage -o $@ -b boot/mbr.bin -no-emul-boot -boot-load-size 1 -boot-info-table iso/
	rm -rf iso

# ======================================
# Disk image
# ======================================
disk.img:
	qemu-img create -f raw disk.img 512M

# ======================================
# Run targets
# ======================================
run-iso: iso-image.iso disk.img
	qemu-system-i386 -cdrom $< -hda disk.img

# Debugging
kernel.elf: $(BUILD_DIR)/boot/kernel_entry.o $(filter-out $(BUILD_DIR)/boot/kernel_entry.o,$(OBJ_FILES))
	@mkdir -p $(dir $@)
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^

debug: build/kernel/kernel.bin kernel.elf
	qemu-system-i386 -s -S -cdrom build/kernel/kernel.bin -d guest_errors,int &
	x86_64-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# ======================================
# Utilities
# ======================================
echo: build/kernel/kernel.bin
	xxd $<

# Disassemble
%.dis: %.bin
	ndisasm -b 32 $< > $@

# ======================================
# Clean
# ======================================
clean:
	$(RM) -r $(BUILD_DIR) *.bin *.elf *.dis *.iso *.img
