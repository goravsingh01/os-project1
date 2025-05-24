# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = $(SRC_DIR)/include

# Tools
AS = nasm
CC = i386-elf-gcc
LD = i386-elf-ld

# Flags
CFLAGS = -ffreestanding -m32 -O2 -Wall -Wextra -I$(INCLUDE_DIR)
ASFLAGS = -f elf32

# Files
BOOT = $(SRC_DIR)/boot/bootloader.asm
KERNEL_C = $(SRC_DIR)/kernel
ASM = $(SRC_DIR)/asm

OBJS = \
  $(BUILD_DIR)/boot.o \
  $(BUILD_DIR)/asm/isr.o \
  $(BUILD_DIR)/kernel/kernel.o \
  $(BUILD_DIR)/kernel/io.o \
  $(BUILD_DIR)/kernel/screen.o \
  $(BUILD_DIR)/kernel/idt.o \
  $(BUILD_DIR)/kernel/isr.o \
  $(BUILD_DIR)/kernel/irq.o \
  $(BUILD_DIR)/kernel/keyboard.o \
  $(BUILD_DIR)/kernel/shell.o \
  $(BUILD_DIR)/kernel/string.o

# Target
KERNEL_BIN = kernel.bin

all: $(KERNEL_BIN)

$(KERNEL_BIN): $(OBJS)
	$(LD) -T linker.ld -nostdlib -o $@ $(OBJS)

# Assembly
$(BUILD_DIR)/boot.o: $(BOOT)
	mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/asm/isr.o: $(ASM)/isr.asm
	mkdir -p $(BUILD_DIR)/asm
	$(AS) $(ASFLAGS) $< -o $@

# C source compilation
$(BUILD_DIR)/kernel/%.o: $(KERNEL_C)/%.c
	mkdir -p $(BUILD_DIR)/kernel
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(KERNEL_BIN)

run: all
	qemu-system-i386 -kernel $(KERNEL_BIN)
