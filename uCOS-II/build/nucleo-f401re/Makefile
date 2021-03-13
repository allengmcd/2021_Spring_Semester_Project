#-----------------------------------------------------------------------------
# project name
#
PROJECT     := main

#-----------------------------------------------------------------------------
# project directories
#

# location of the QP/C framework (if not provided in an environemnt var.)
ifeq ($(ROOT),)
ROOT := ../../
endif

# QP port used in this project
UCOS_II := ../../Micrium/Software/uCOS-II

# list of all source directories used by this project
VPATH = \
	.. \
	$(ROOT)/src \
	$(ROOT)/src/uCOS \
	$(ROOT)/BSP \
	$(ROOT)/BSP/CMSIS \
	$(ROOT)/BSP/ST/StdPeripheralDrivers \
	$(ROOT)/Util \
	$(UCOS_II)/Source \
	$(UCOS_II)/ARM-Cortex-M4/GNU
	

# list of all include directories needed by this project
INCLUDES  = \
	-I.. \
	-I$(ROOT)/include \
	-I$(ROOT)/src \
	-I$(ROOT)/src/uCOS \
	-I$(ROOT)/BSP \
	-I$(ROOT)/BSP/CMSIS \
	-I$(ROOT)/BSP/ST/StdPeripheralDrivers \
	-I$(ROOT)/Util \
	-I$(UCOS_II)/Source \
	-I$(UCOS_II)/ARM-Cortex-M4/GNU

#-----------------------------------------------------------------------------
# files
#

# assembler source files
ASM_SRCS := \
	os_cpu_a.s \
	startup_stm32f401xe.s

# C source files
C_SRCS := \
	main.c \
	app_hooks.c

OUTPUT    := $(PROJECT)
LD_SCRIPT := LinkerScript.ld


RTOS_SRCS := \
	os_core.c \
	os_flag.c \
	os_mbox.c \
	os_mem.c \
	os_mutex.c \
	os_q.c \
	os_sem.c \
	os_task.c \
	os_time.c \
	os_tmr.c \
	os_cpu_c.c
	
BSP_SRCS := \
	hw_init.c \
	bsp.c \
	bspUart.c
	
ST_PERIPHERAL_DRIVERS_SRCS := \
	misc.c \
	stm32f4xx_gpio.c \
	stm32f4xx_rcc.c \
	stm32f4xx_spi.c \
	stm32f4xx_usart.c 

CMSIS_SRCS := system_stm32f2xx.c

UTIL_SRCS := \
	print.c \
	printf.c 
	

LIB_DIRS  := 
LIBS      := 

# defines
DEFINES   := -DTARGET_IS_TM4C123_RB1 -D__FPU_PRESENT

# ARM CPU, ARCH, FPU, and Float-ABI types...
# ARM_CPU:   [cortex-m0 | cortex-m0plus | cortex-m1 | cortex-m3 | cortex-m4]
# ARM_ARCH:  [6 | 7] (NOTE: must match ARM_CPU!)
# ARM_FPU:   [ | vfp]
# FLOAT_ABI: [ | soft | softfp | hard]
#
ARM_CPU   := -mcpu=cortex-m4
ARM_ARCH  := 7   # NOTE: must match the ARM_CPU!
ARM_FPU   := -mfpu=vfp
FLOAT_ABI := -mfloat-abi=softfp
ASM_FPU   := -defsym=__FPU_PRESENT=1

#-----------------------------------------------------------------------------
# GNU-ARM toolset (NOTE: You need to adjust to your machine)
# see https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
#
GNU_ARM := C:/"Program Files (x86)"/"GNU Arm Embedded Toolchain"/"9 2020-q2-update"/


CC    := $(GNU_ARM)/bin/arm-none-eabi-gcc
AS    := $(GNU_ARM)/bin/arm-none-eabi-as
LINK  := $(GNU_ARM)/bin/arm-none-eabi-g++
BIN   := $(GNU_ARM)/bin/arm-none-eabi-objcopy


##############################################################################
# Typically, you should not need to change anything below this line

# basic utilities (included in Qtools for Windows), see:
#    http://sourceforge.net/projects/qpc/files/Qtools

MKDIR := mkdir
RM    := rm

#-----------------------------------------------------------------------------
# build options for various configurations for ARM Cortex-M4F
#

# combine all the soruces...
C_SRCS += $(RTOS_SRCS)
C_SRCS += $(BSP_SRCS)
C_SRCS += $(ST_PERIPHERAL_DRIVERS_SRCS)
C_SRCS += $(CMSIS_SRCS)
C_SRCS += $(UTIL_SRCS)

ifeq (rel, $(CONF)) # Release configuration ..................................

BIN_DIR := rel

ASFLAGS = $(ARM_CPU) $(ARM_FPU) $(ASM_CPU) $(ASM_FPU)

CFLAGS = -c $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O1 $(INCLUDES) $(DEFINES) -DNDEBUG

else ifeq (spy, $(CONF))  # Spy configuration ................................

BIN_DIR := spy

ASFLAGS = -g $(ARM_CPU) $(ARM_FPU) $(ASM_CPU) $(ASM_FPU)

CFLAGS = -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES) -DQ_SPY

else # default Debug configuration ..........................................

BIN_DIR := dbg

ASFLAGS = -g $(ARM_CPU) $(ARM_FPU) $(ASM_CPU) $(ASM_FPU)

CFLAGS = -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES)

endif # ......................................................................


LINKFLAGS = -T$(LD_SCRIPT) $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb \
	-specs=nosys.specs -specs=nano.specs \
	-Wl,-Map,$(BIN_DIR)/$(OUTPUT).map,--cref,--gc-sections $(LIB_DIRS)


ASM_OBJS     := $(patsubst %.s,%.o,  $(notdir $(ASM_SRCS)))
C_OBJS       := $(patsubst %.c,%.o,  $(notdir $(C_SRCS)))

TARGET_BIN   := $(BIN_DIR)/$(OUTPUT).bin
TARGET_ELF   := $(BIN_DIR)/$(OUTPUT).elf
ASM_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(ASM_OBJS))
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o, %.d, $(C_OBJS_EXT))

# create $(BIN_DIR) if it does not exist
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif

#-----------------------------------------------------------------------------
# rules
#

all: $(TARGET_BIN)
#all: $(TARGET_ELF)

$(TARGET_BIN): $(TARGET_ELF)
	$(BIN) -O binary $< $@

$(TARGET_ELF) : $(ASM_OBJS_EXT) $(C_OBJS_EXT)
	$(LINK) $(LINKFLAGS) -o $@ $^  $(LIBS)

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

# include dependency files only if our goal depends on their existence
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
-include $(C_DEPS_EXT)
  endif
endif


.PHONY : clean
clean:
	-$(RM) $(BIN_DIR)/*.o \
	$(BIN_DIR)/*.d \
	$(BIN_DIR)/*.bin \
	$(BIN_DIR)/*.elf \
	$(BIN_DIR)/*.map
	
show:
	@echo PROJECT = $(PROJECT)
	@echo CONF = $(CONF)
	@echo DEFINES = $(DEFINES)
	@echo ASM_FPU = $(ASM_FPU)
	@echo ASM_SRCS = $(ASM_SRCS)
	@echo C_SRCS = $(C_SRCS)
	@echo ASM_OBJS_EXT = $(ASM_OBJS_EXT)
	@echo C_OBJS_EXT = $(C_OBJS_EXT)
	@echo C_DEPS_EXT = $(C_DEPS_EXT)
	@echo TARGET_ELF = $(TARGET_ELF)