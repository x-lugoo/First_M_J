################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ui/main_ui/sm_menu.c \
../src/ui/main_ui/sm_menu_config.c \
../src/ui/main_ui/sm_resource.c \
../src/ui/main_ui/sm_save.c \
../src/ui/main_ui/sm_tool.c \
../src/ui/main_ui/sm_tree.c 

OBJS += \
./src/ui/main_ui/sm_menu.o \
./src/ui/main_ui/sm_menu_config.o \
./src/ui/main_ui/sm_resource.o \
./src/ui/main_ui/sm_save.o \
./src/ui/main_ui/sm_tool.o \
./src/ui/main_ui/sm_tree.o 


# Each subdirectory must supply rules for building sources it contributes
src/ui/main_ui/%.o: ../src/ui/main_ui/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Compiler'
	"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/bin/arm-none-linux-gnueabi-gcc" -O0 -g2 -Wall -funwind-tables -I"D:/pax/2.8.13_prolin/MiniPos/inc" -I"D:/pax/2.8.13_prolin/MiniPos/src/jansson" -I"D:/pax/2.8.13_prolin/MiniPos/src/curl" -I"D:/pax/2.8.13_prolin/MiniPos/src" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/platforms/paxngfp_201205/include" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/platforms/paxngfp_201205/include/freetype2" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/arm-none-linux-gnueabi/libc/usr/include" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/lib/gcc/arm-none-linux-gnueabi/4.6.3/include" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


