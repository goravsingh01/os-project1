# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = $(SRC_DIR)/include
BOOT_DIR = $(SRC_DIR)/boot
KERNEL_DIR = $(SRC_DIR)/kernel

# Tools
AS = nasm
CC = i386-elf-gcc
LD = i386-elf-ld

# Flags
CFLAGS = -ffreestanding -m32 -O2 -Wall -Wextra -I$(INCLUDE_DIR)
ASFLAGS = -f elf32

# Files
BOOT_ASM = $(BOOT_DIR)/boot.asm
KERNEL_ENTRY = $(BOOT_DIR)/kernel_entry.asm

# Object files
OBJS = \
  $(BUILD_DIR)/boot.o \
  $(BUILD_DIR)/kernel_entry.o \
  $(BUILD_DIR)/kernel/kernel.o \
  $(BUILD_DIR)/kernel/io.o \
  $(BUILD_DIR)/kernel/screen.o \
  $(BUILD_DIR)/kernel/keyboard.o \
  $(BUILD_DIR)/kernel/shell.o \
  $(BUILD_DIR)/kernel/string.o \
  $(BUILD_DIR)/kernel/filesystem.o

# Target
KERNEL_BIN = kernel.bin

all: $(KERNEL_BIN)

$(KERNEL_BIN): $(OBJS)
	$(LD) -T linker.ld -nostdlib -o $@ $(OBJS)

$(BUILD_DIR)/boot.o: $(BOOT_ASM)
	mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/kernel_entry.o: $(KERNEL_ENTRY)
	mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/kernel/%.o: $(KERNEL_DIR)/%.c
	mkdir -p $(BUILD_DIR)/kernel
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(KERNEL_BIN)

run: all
	qemu-system-i386 -kernel $(KERNEL_BIN)
