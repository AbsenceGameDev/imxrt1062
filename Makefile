# Note: has not been tested in any linux distro for some while. Recent changes may cause build issues on other platforms than win/NT.	

# Remember to build or install the arm eabi toolset before trying to build this project with make
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
LOADER = teensy_loader_cli

# Language standard
STD=-std=c99

# Cheap way to pass in arbitrary flags
V=

OUTFILE = firmware

BUILD_DIR = ./build
SRC_DIRS ?= ./TBM_CC/Core/src ./TBM_CC/teensy ./TBM_CC/Core/include

# Breaking up the shell find command makes so it can compile on both Windows and Linux
SRCS := $(shell find "./TBM_CC/Core/src" -name *.c -or -name *.s)
SRCS += $(shell find "./TBM_CC/teensy" -name *.c -or -name *.s)
SRCS += $(shell find "./TBM_CC/Core/include" -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS  := $(shell find "./TBM_CC/Core/src" -type d)
INC_DIRS  += $(shell find "./TBM_CC/teensy" -type d)
INC_DIRS  += $(shell find "./TBM_CC/Core/include" -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

FPU_FLAGS : -mfloat-abi=hard -mfpu=fpv5-d16 ## Does not work?

INITOPTS = -Wl,--gc-sections,--print-gc-sections,--print-memory-usage -nostdlib -nostartfiles
EXTMEMOPTS = -Wl,--defsym=__heap_start=0x20200000,--defsym=__heap_end=0x2027ffff
LDSCRIPT_PATH = -TTBM_CC/teensy/imxrt1062.ld

CFLAGS = $(V) -O3 -Wall -Wa,-Iinc -Werror -Wno-error=unused-variable -mcpu=cortex-m7 -std=c99 $(FPU_FLAGS) -mthumb $(INC_FLAGS)
LDFLAGS = $(INITOPTS)
LDFLAGS += $(EXTMEMOPTS) 
LDFLAGS += $(LDSCRIPT_PATH)

$(BUILD_DIR)/$(OUTFILE).hex: $(BUILD_DIR)/$(OUTFILE).elf
	@echo \>\> Creating .hex
	@$(OBJCOPY) -O ihex -R .eeprom build/$(OUTFILE).elf build/$(OUTFILE).hex
	@$(OBJDUMP) -d -x build/$(OUTFILE).elf > build/$(OUTFILE).dis
	@$(OBJDUMP) -d -S -C build/$(OUTFILE).elf > build/$(OUTFILE).lst
	@$(SIZE) build/$(OUTFILE).elf

$(BUILD_DIR)/$(OUTFILE).elf: $(OBJS)
	@echo \>\> Creating .elf
	@$(CC) $(CFLAGS) -Xlinker -Map=build/$(OUTFILE).map $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.s.o: %.s
	@echo \>\> Creating .s.o
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	@echo \>\> Creating .c.o
	@$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: flashnew
flashnew: 
	@echo \>\>
	@echo \>\> Removing /build dir..
	@$(RM) -f -r $(BUILD_DIR)

	@echo \>\>
	@echo \>\> Beginning building..!
	make

	@echo \>\>
	@echo \>\> Beginning flashing process..
	$(LOADER) --mcu=TEENSY41 -w -s -v $(BUILD_DIR)/$(OUTFILE).hex
	@echo \>\> Device has been flashed!

.PHONY: flash
flash: 
	@echo \>\>
	@echo \>\> Beginning building..
	make

	@echo \>\>
	@echo \>\> Beginning flashing process..
	
	@echo \>\>
	$(LOADER) --mcu=TEENSY41 -w -s -v $(BUILD_DIR)/$(OUTFILE).hex
	@echo "Device has been flashed!"

.PHONY: flashonly
flashonly:
	@echo \>\>
	@echo \>\> Beginning flashing process..
	
	@echo \>\>
	$(LOADER) --mcu=TEENSY41 -w -s -v $(BUILD_DIR)/$(OUTFILE).hex
	@echo \>\> Device has been flashed!

.PHONY: delete
delete:
	@echo \>\>
	@echo \>\> Deleting /build dir..
	@$(RM) -f -r $(BUILD_DIR)

.PHONY: clean
clean:
	@echo \>\>
	@echo \>\> Deleting /build dir..
	@$(RM) -f -r $(BUILD_DIR)
	
	@echo \>\>
	@echo \>\> Beginning building..
	make

MKDIR_P ?= mkdir -p