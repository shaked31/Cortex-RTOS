# Project Config
PROJECT_NAME = Cortex-RTOS
CPU = cortex-a8
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

# Dirs
ARCH_DIR = arch/arm/v7-a
SCRIPT_DIR = scripts
APP_DIR = apps
BUILD_DIR = build

# Flags
CFLAGS = -mcpu=$(CPU) -g -Wall -nostdlib -Iinclude
LDFLAGS = -T $(SCRIPT_DIR)/linker.ld

# Files
SRC_S = $(ARCH_DIR)/startup.s
SRC_C = $(APP_DIR)/main.c

OBJS = $(BUILD_DIR)/startup.o $(BUILD_DIR)/main.o

all: $(BUILD_DIR) $(BUILD_DIR)/kernel.elf

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/startup.o: $(ARCH_DIR)/startup.s
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(APP_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	$(OBJCOPY) -O binary $< $@


run: all
	qemu-system-arm -M realview-pb-a8 -kernel $(BUILD_DIR)/kernel.elf -nographic

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean