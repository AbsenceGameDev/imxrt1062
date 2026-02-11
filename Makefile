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
##V:=$(filter-out $@,$(MAKECMDGOALS))

OUTFILE = firmware

BUILD_DIR = ./build
SRC_DIRS ?= ./TBM_CC/Core/src ./TBM_CC/teensy ./TBM_CC/Core/include ./TBM_CC/Core/tests

# Breaking up the shell find command makes so it can compile on both Windows and Linux
SRCS := $(shell find "./TBM_CC/Core/src" -name *.c -or -name *.s)
SRCS += $(shell find "./TBM_CC/teensy" -name *.c -or -name *.s)
SRCS += $(shell find "./TBM_CC/Core/include" -name *.c -or -name *.s)
SRCS += $(shell find "./TBM_CC/Core/tests" -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS  := $(shell find "./TBM_CC/Core/src" -type d)
INC_DIRS  += $(shell find "./TBM_CC/teensy" -type d)
INC_DIRS  += $(shell find "./TBM_CC/Core/include" -type d)
INC_DIRS  += $(shell find "./TBM_CC/Core/tests" -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

FPU_FLAGS=-mfloat-abi=hard -mfpu=fpv5-d16
ARM_FLAGS=-mcpu=cortex-m7 ${FPU_FLAGS} -mthumb
ERR_FLAGS=-Werror -Wno-error=unused-variable -Wno-format##-Wnull-dereference
BASERULE_FLAGS=-Wall -std=c99 $(ARM_FLAGS) $(ERR_FLAGS)##-flto Link-time optimization is removing some code it seems and is causing things to break

DATA_FLAGS=-fdata-sections -ffunction-sections -fallow-store-data-races -fno-common
EXTRA_COMPILE_FLAGS=$(DATA_FLAGS) -fstack-usage -ffast-math
#CFLAGS=$(V) -O3 $(BASERULE_FLAGS) $(EXTRA_COMPILE_FLAGS) -Wa,-Iinc $(INC_FLAGS)
CFLAGS=-O3 $(BASERULE_FLAGS) $(EXTRA_COMPILE_FLAGS) -Wa,-Iinc $(INC_FLAGS)


INITOPTS = -Wl,--gc-sections,--print-gc-sections,--print-memory-usage -nostdlib -nostartfiles
EXTMEMOPTS = -Wl,--defsym=__heap_start=0x20200000,--defsym=__heap_end=0x2027ffff
LDSCRIPT_PATH = -TTBM_CC/teensy/imxrt1062.ld
LDFLAGS = $(INITOPTS) $(EXTMEMOPTS) $(LDSCRIPT_PATH)


## Formatting/Colouring - START
GRN=\e[32m
YLW=\e[33m
BLU=\e[34m
CYN=\e[36m
LGR=\e[92m

BG0=\e[100m
BG1=\e[104m
BGX=\e[40m
END=\e[0m

define CMsg0
	@echo -e "${1}${2} \>\> $3 ${END}"
endef
define CMsg1
	@echo -e "${1}${2} \>\> $3 $4 ${END}"
endef
define CMsg2
	@echo -e "${1}${2} \>\> ${END}${3}${2}$4${END}${1}${2}$5${END}"
endef
## Formatting/Colouring - END

$(BUILD_DIR)/$(OUTFILE).hex: $(BUILD_DIR)/$(OUTFILE).elf
	$(call CMsg1, ${BLU}, ${BG1},Creating .hex - EXECUTE EABI-OBJ-COPY, (1/4))
	@$(OBJCOPY) -O ihex -R .eeprom build/$(OUTFILE).elf build/$(OUTFILE).hex
	
	$(call CMsg1, ${BLU}, ${BG1},Creating .hex - EXECUTE EABI-OBJ-DUMP to '.dis', (2/4))
	@$(OBJDUMP) -d -x build/$(OUTFILE).elf > build/$(OUTFILE).dis

	$(call CMsg1, ${BLU}, ${BG1},Creating .hex - EXECUTE EABI-OBJ-DUMP to '.lst', (3/4))
	@$(OBJDUMP) -d -S -C build/$(OUTFILE).elf > build/$(OUTFILE).lst
	
	$(call CMsg1, ${BLU}, ${BG1},Creating .hex - EXECUTE EABI-SIZE, (4/4))
	@$(SIZE) build/$(OUTFILE).elf

$(BUILD_DIR)/$(OUTFILE).elf: $(OBJS)
	$(call CMsg1, ${BLU}, ${BG1},Creating .elf - Compling .ELF with linker map with LDFLAGS, (1/1))

	@$(CC) $(CFLAGS) -Xlinker -Map=build/$(OUTFILE).map $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.s.o: %.s
	$(call CMsg0, ${BLU}, ${BG1},Creating .s.o)
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	$(call CMsg0, ${BLU},${BG1},Compiling Object (Generate a .c.o file in $(dir $@)))

	@$(MKDIR_P) $(dir $@)

	${CC} ${CFLAGS} -c $< -o $@

.PHONY: flashnew
flashnew: 
	$(call CMsg0, ${CYN})
	$(call CMsg0, ${YLW},${BG0},Removing /build dir.. )

	@$(RM) -f -r $(BUILD_DIR)

	$(call CMsg0, ${CYN})
	$(call CMsg0, ${YLW},${BG0},Beginning building..! )

	make

	$(call CMsg0, ${CYN})
	$(call CMsg0, ${YLW},${BG0},Beginning flashing process.. )

	$(LOADER) --mcu=TEENSY41 -w -s -v $(BUILD_DIR)/$(OUTFILE).hex
	$(call CMsg0, ${LGR},${BG0},Device has been flashed! )

.PHONY: flash
flash: 
	$(call CMsg0, ${CYN})
	$(call CMsg0, ${YLW},${BG0},Beginning building..! )

	make

	$(call CMsg0, ${CYN})
	$(call CMsg0, ${YLW},${BG0},Beginning flashing process.. )
	
	$(call CMsg0, ${CYN})
	$(LOADER) --mcu=TEENSY41 -w -s -v $(BUILD_DIR)/$(OUTFILE).hex
	$(call CMsg0, ${LGR},${BG0},Device has been flashed! )

.PHONY: flashonly
flashonly:
	$(call CMsg0, ${CYN})
	$(call CMsg0, ${YLW},${BG0},Beginning flashing process.. )
	
	$(call CMsg0, ${CYN})
	$(LOADER) --mcu=TEENSY41 -w -s -v $(BUILD_DIR)/$(OUTFILE).hex
	$(call CMsg0, ${LGR},${BG0},Device has been flashed! )

.PHONY: delete
delete:
	$(call CMsg0, ${CYN})
	$(call CMsg0, ${YLW},${BG0},Removing /build dir.. )

	@$(RM) -f -r $(BUILD_DIR)

.PHONY: clean
clean:
	$(call CMsg0, ${CYN})
	$(call CMsg0, ${YLW},${BG0},Removing /build dir.. )
	@$(RM) -f -r $(BUILD_DIR)
	
	$(call CMsg0, ${CYN})
	$(call CMsg0, ${YLW},${BG0},Beginning building..! )
	make

MKDIR_P ?= mkdir -p