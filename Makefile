# define build folder
BUILD_DIR := build

# detect all .o files based on their .c source
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h  drivers/*.h cpu/*.h)

# update OBJ_FILES to build/ folder
OBJ_FILES := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES)) $(BUILD_DIR)/cpu/interrupt.o

CC = x86_64-elf-gcc
LD = x86_64-elf-ld


# kernel.bin target now outputs to build/
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/boot/kernel_entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# os-image.bin target
$(BUILD_DIR)/os-image.bin: $(BUILD_DIR)/boot/mbr.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

run: $(BUILD_DIR)/os-image.bin
	qemu-system-i386 -fda $< -hda disk.img -display sdl -boot a

run-wsl: $(BUILD_DIR)/os-image.bin # yep im a poor dude on windows 10
	cmd.exe /c qemu-system-i386 -fda $< -hda disk.img -display sdl -boot a 
# removed serial so we dont spam the terminal
# only for debug
kernel.elf: $(BUILD_DIR)/boot/kernel_entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^

debug: $(BUILD_DIR)/os-image.bin kernel.elf
	qemu-system-i386 -s -S -fda $< -d guest_errors,int &
	i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# compilation rules
$(BUILD_DIR)/%.o: %.c ${HEADERS}
	@mkdir -p $(dir $@)
	$(CC) -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c $< -o $@

$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	nasm $< -f elf -o $@

$(BUILD_DIR)/%.bin: %.asm
	@mkdir -p $(dir $@)
	nasm $< -f bin -o $@

clean:
	$(RM) build/*.bin build/*.o build/*.dis build/*.elf
	$(RM) build/kernel/*.o
	$(RM) build/boot/*.o build/boot/*.bin
	$(RM) build/drivers/*.o
	$(RM) build/cpu/*.o