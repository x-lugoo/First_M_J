################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/ui/settle_ui/subdir.mk
-include src/ui/main_ui/subdir.mk
-include src/cdz_ui/settle_menu/subdir.mk
-include src/cdz_ui/payment_menu/subdir.mk
-include src/cdz_ui/data/subdir.mk
-include src/cdz_ui/component/subdir.mk
-include src/cdz_ui/common_menu/subdir.mk
-include src/cdz_ui/book_menu/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
NOSTRIP += \
app.nostrip \


# All Target
all: MiniPos

# Tool invocations
MiniPos: $(NOSTRIP)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Strip'
	"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/bin/arm-none-linux-gnueabi-strip" -g $(NOSTRIP) -o"MiniPos"
	@echo 'Finished building target: $@'
	@echo ' '

app.nostrip: $(OBJS) $(USER_OBJS)
	@echo 'Invoking: GCC Linker'
	"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/bin/arm-none-linux-gnueabi-gcc" -o"app.nostrip" $(OBJS) $(USER_OBJS) $(LIBS) -L"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/platforms/paxngfp_201205/lib" -losal -Wl,-rpath=//opt/lib -Wl,-rpath=./lib -Wl,-rpath-link,"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/platforms/paxngfp_201205/lib" -lcrypto -lcurl -ljson -lfreetype -lpng -lqueue -lpthread -lts -lxui -L"D:/pax/2.8.13_prolin/MiniPos/lib"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(NOSTRIP) MiniPos
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
