
######################################
# target
######################################
TARGET = CS_EPS_PMM_N_SH


# Set the FLAG OS_SYSTEM - LINUX or MAC_OS or WINDOWS it depends which sistem are you use to build.
BUILD_OS_SYSTEM = WINDOWS


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# pathsSrc/syscalls.c \
#######################################
# Build path
BUILD_DIR = Build_gcc_eabi

######################################
# source
######################################
# C sources
C_SOURCES =  \
Core/Src/main.c \
Core/Src/Error_Handler.c \
Core/Src/SetupPeriph.c \
Core/Src/i2c_comm.c \
Core/Src/uart_comm.c \
Core/Src/uart_terminal.c \
Core/Src/median_filter.c \
Core/Src/Fn_CRC16.c \
Core/Src/tim_pwm.c \
Core/Src/TMP1075.c \
Core/Src/FRAM.c \
Core/Src/PCA9534.c \
Core/Src/TCA9539.c \
Core/Src/ADS1015.c \
Core/Src/TCA9548.c \
Core/Src/TCA6424A.c \
Core/Src/INA231.c \
Core/Src/PDM/pdm_config.c \
Core/Src/PDM/pdm_init_IC.c \
Core/Src/PDM/pdm_init.c \
Core/Src/PDM/pdm_ctrl.c \
Core/Src/PDM/pdm.c \
Core/Src/PMM/pmm_config.c \
Core/Src/PMM/pmm_init_IC.c \
Core/Src/PMM/pmm_init.c \
Core/Src/PMM/pmm_ctrl.c \
Core/Src/PMM/pmm_savedata.c \
Core/Src/PMM/pmm.c \
Core/Src/CAND/CAN.c \
Core/Src/CAND/CAN_cmd.c \
Core/Src/stm32l4xx_it.c \
Core/Src/syscalls.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_dma.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_exti.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_gpio.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_i2c.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_pwr.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_rcc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_rtc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_tim.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_lpuart.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usart.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_utils.c \
Src/system_stm32l4xx.c

# ASM sources
ASM_SOURCES =  \
Core/startup/startup_stm32l496qgix.s


#######################################
# binaries
#######################################
ifeq ($(BUILD_OS_SYSTEM),LINUX)
GCC_PATH = /opt/gcc-arm-none-eabi-7-2017-q4-major/bin
else ifeq ($(BUILD_OS_SYSTEM),MAC_OS)
GCC_PATH = /Users/Ivan/Development/opt/gcc-arm-none-eabi-7-2018-q2-update/bin
else ifeq ($(BUILD_OS_SYSTEM),WINDOWS)
GCC_PATH_WIN = C:\ST\STM32CubeIDE_1.3.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.7-2018-q2-update.win32_1.0.0.201904181610\tools\bin
endif

PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else ifdef GCC_PATH_WIN
CC = $(GCC_PATH_WIN)/$(PREFIX)gcc.exe
AS = $(GCC_PATH_WIN)/$(PREFIX)gcc.exe -x assembler-with-cpp
CP = $(GCC_PATH_WIN)/$(PREFIX)objcopy.exe
SZ = $(GCC_PATH_WIN)/$(PREFIX)size.exe
else
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_FULL_LL_DRIVER \
-DHSE_VALUE=16000000 \
-DHSE_STARTUP_TIMEOUT=100 \
-DLSE_STARTUP_TIMEOUT=5000 \
-DLSE_VALUE=32768 \
-DMSI_VALUE=16000000 \
-DEXTERNALSAI1_CLOCK_VALUE=2097000 \
-DHSI_VALUE=16000000 \
-DLSI_VALUE=32000 \
-DVDD_VALUE=3300 \
-DPREFETCH_ENABLE=0 \
-DINSTRUCTION_CACHE_ENABLE=1 \
-DDATA_CACHE_ENABLE=1 \
-DSTM32L496xx\
-DEBUGprintf


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ICore/Inc \
-ICore/Inc/PMM \
-ICore/Inc/PDM \
-ICore/Inc/PAM \
-ICore/Inc/CAND \
-ICore/Inc/FLASH \
-ICore/Inc/BATT \
-IDrivers/STM32L4xx_HAL_Driver/Inc \
-IDrivers/CMSIS/Device/ST/STM32L4xx/Include \
-IDrivers/CMSIS/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32L496QGIX_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys -lcanv_lib -lFlash_lib
LIBDIR = -L./Core/Src/LIB
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
