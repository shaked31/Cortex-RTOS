# Project Config
PROJECT_NAME = Cortex-RTOS
CPU = cortex-a8
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

# Dirs
SCRIPT_DIR = scripts
BUILD_DIR = build
INCLUDE_DIR = include
SRC_DIRS = arch/arm/v7-a apps drivers lib kernel

# Files
SRC_C = $(shell find $(SRC_DIRS) -name '*.c')
SRC_S = $(shell find $(SRC_DIRS) -name '*.s')

OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRC_C:.c=.o)))
OBJS += $(addprefix $(BUILD_DIR)/, $(notdir $(SRC_S:.s=.o)))

# Flags
CFLAGS = -mcpu=$(CPU) -g -Wall -nostdlib -I$(INCLUDE_DIR) -fno-builtin
LDFLAGS = -T $(SCRIPT_DIR)/linker.ld
LIBGCC = $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

vpath %.c $(SRC_DIRS)
vpath %.s $(SRC_DIRS)


all: $(BUILD_DIR) $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBGCC) -o $@
	
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	$(OBJCOPY) -O binary $< $@


run: all
	qemu-system-arm -M realview-pb-a8 -kernel $(BUILD_DIR)/kernel.elf -audio none -nographic

qemu: all
	qemu-system-arm -M realview-pb-a8 -kernel $(BUILD_DIR)/kernel.elf -audio none
	
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean