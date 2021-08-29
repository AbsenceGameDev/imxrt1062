CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
LOADER = teensy_loader_cli

OUTFILE = firmware

BUILD_DIR = ./build
SRC_DIRS ?= ./TBM_CC/Core/src ./TBM_CC/teensy ./TBM_CC/Core/include

SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS = -O3 -Wall -Werror -Wno-error=unused-variable -mcpu=cortex-m7 -mthumb $(INC_FLAGS)
LDFLAGS = -Wl,--gc-sections,--print-gc-sections,--print-memory-usage -nostdlib -nostartfiles -TTBM_CC/teensy/imxrt1062.ld

$(BUILD_DIR)/$(OUTFILE).hex: $(BUILD_DIR)/$(OUTFILE).elf
	@$(OBJCOPY) -O ihex -R .eeprom build/$(OUTFILE).elf build/$(OUTFILE).hex
	@$(OBJDUMP) -d -x build/$(OUTFILE).elf > build/$(OUTFILE).dis
	@$(OBJDUMP) -d -S -C build/$(OUTFILE).elf > build/$(OUTFILE).lst
	@$(SIZE) build/$(OUTFILE).elf

$(BUILD_DIR)/$(OUTFILE).elf: $(OBJS)
	@$(CC) $(CFLAGS) -Xlinker -Map=build/$(OUTFILE).map $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.s.o: %.s
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
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