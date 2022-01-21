
# Make phony targets .PHONY
# There isn't really an all file created and there really isn't clean file.
# There are actually just tags that identify a target whose rules should
# always be run.
.PHONY: all clean target

# [SD]
# 1: enable SD 0: disable
SD = STORAGE_SD=1

# [LCD Resolution]
# __800x480__ __320x240__
LCD_RESOLUTION = __800x480__

# The Preprocessor Macros
DEFS = \
	-DN9H26K6=1 \
	-D$(SD) \
	-D$(LCD_RESOLUTION) \

# [Include files]
INCLUDES = \
	-I../../../../../../../BSP/Driver/Include \
	-I../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application \
	-I../../../../../../../BSP/SampleCode/emWin/GUIDemo/tslib \
	-I../../../../../../../BSP/ThirdParty/emWin/Config \
	-I../../../../../../../BSP/ThirdParty/emWin/Include \

GFLAGS = \
	-mcpu=arm926ej-s \
	-marm \
	-mlittle-endian \
	-Os \
	-fmessage-length=0 \
	-fsigned-char \
	-ffunction-sections \
	-fdata-sections \
	-g \

CFLAGS = $(DEFS) $(INCLUDES) -std=gnu11 

CPPFLAGS =
CXXFLAGS =

ASFLAG = -g
TEMPLTFILE = ../../../../../../../BSP/Driver/Source/gcc_arm_SRAM.ld
LINKFILE = ../../../../../../../BSP/Driver/Source/gcc_arm_SRAM_N9H26K6_64MB.ld
LDFLAGS = 

# [LIBS]
LIBS = \
	-lN9H26_SYS \
	-lN9H26_VPOST_FW050TFT_800x480 \
	-lN9H26_ADC \
	-lN9H26_SIC \
	-lN9H26_GNAND \
	-lN9H26_NVTFAT \
	-lNUemWin_ARM9_GNU \
	-lN9H26_BLT \
	-lN9H26_JPEG \
	-lm \
	-L"../../../../../../../BSP/Library_GCC/IPLib" \
	-L"../../../../../../../BSP/Library_GCC/GNANDLib" \
	-L"../../../../../../../BSP/Library_GCC/NVTFATLib" \
	-L"../../../../../../../BSP/ThirdParty/emWin/Lib" \


# define the C source files
# Don't include paths in filenames
# Inerting and maintaining paths everywhere is tedious and error prone. Better is to use
# VPATH within the Makefile like this:
#	VPATH=../src
SRCS_C = \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/main.c \
		\
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/tslib/N9H26TouchPanel.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/tslib/fbutils.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/tslib/testutils.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/tslib/ts_calibrate.c \
		\
	../../../../../../../BSP/ThirdParty/emWin/Config/GUI_X.c \
	../../../../../../../BSP/ThirdParty/emWin/Config/LCDConf.c \
		\
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIConf.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_AntialiasedText.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Automotive.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_BarGraph.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Bitmap.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_ColorBar.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Conf.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Cursor.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Fading.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Graph.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_IconView.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_ImageFlow.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Intro.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Listview.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_RadialMenu.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Resource.c\
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Skinning.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Speed.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Speedometer.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Start.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_TransparentDialog.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_Treeview.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_VScreen.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_WashingMachine.c \
	../../../../../../../BSP/SampleCode/emWin/GUIDemo/Application/GUIDEMO_ZoomAndRotate.c \


# [Assembly Files]
SRCS_S = ../../../../../../../BSP/Driver/Source/Startup_GCC.S


# Explicitly name sources
# Lines like this one should be avoided.
# OS_C_SRCS := $(wildcard *.c)


# define the C object files
#
# This uses Suffix Replacement within a macro:
#	$(name:string1=string2)
#		For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macros SRCS_C
# with the .o suffix
OBJS = $(SRCS_C:.c=.o) $(SRCS_S:.S=.o)

C_DEPS = $(OBJS:.o=.d)


MAIN = emWin_GUIDemo.elf

prebuild:
	-arm-none-eabi-cpp -E -DN9H26K6_64MB=1 -P $(TEMPLTFILE) -o $(LINKFILE)

$(MAIN): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C Linker'
	$(CC) $(GFLAGS) -T "$(LINKFILE)" -Xlinker --gc-sections $(LIBDIR) -Wl,-Map,"$*.map" --specs=rdimon.specs -Wl,--start-group $(LIBS) -Wl,--end-group -o "$@" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	

TARGET = emWin_GUIDemo.bin

$(TARGET): $(MAIN)
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	$(OBJCOPY) -O binary "$<" "$@"
	@echo 'Finished building: $@'
	@echo ' '


# This is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $< the name of the prerequisite of 
# the rule (a .c file) and $@ the name of the target of the rule (a .o file)
# (see the GNU make manual section about automatic variables)
# 
# Define an implicit rule
# 
.c.o:
	$(CC) $(GFLAGS) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c "$<" -o "$@"


.S.o:
	$(CC) $(GFLAGS) -x assembler-with-cpp $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


# Define an implicit rule other than above
# The following means to build the target foo.S make should build foo.c

pre-build:
	-arm-none-eabi-cpp -E -DN9H26K6_64MB=1 -P ../../../../../../../BSP/Driver/Source/gcc_arm_SRAM.ld -o $(LINKFILE)

all: pre-build $(TARGET) finalization

finalization: $(MAIN)
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "$<"
	@echo ' '
	
rebuild: clean all
	@echo "rebuild done"
	
clean:
	-$(RM) $(OBJS) $(LINKFILE) $(ASM_DEPS) $(S_UPPER_DEPS) $(C_DEPS) emWin_GUIDemo.elf
	-@echo ' '