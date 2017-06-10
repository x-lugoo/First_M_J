################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cdz_ui/common_menu/button.c \
../src/cdz_ui/common_menu/keypad.c \
../src/cdz_ui/common_menu/menu.c \
../src/cdz_ui/common_menu/page.c \
../src/cdz_ui/common_menu/title.c 

OBJS += \
./src/cdz_ui/common_menu/button.o \
./src/cdz_ui/common_menu/keypad.o \
./src/cdz_ui/common_menu/menu.o \
./src/cdz_ui/common_menu/page.o \
./src/cdz_ui/common_menu/title.o 


# Each subdirectory must supply rules for building sources it contributes
src/cdz_ui/common_menu/%.o: ../src/cdz_ui/common_menu/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Compiler'
	"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/bin/arm-none-linux-gnueabi-gcc" -O0 -g2 -Wall -funwind-tables -I"D:/pax/2.8.13_prolin/MiniPos/inc" -I"D:/pax/2.8.13_prolin/MiniPos/src/jansson" -I"D:/pax/2.8.13_prolin/MiniPos/src/curl" -I"D:/pax/2.8.13_prolin/MiniPos/src" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/platforms/paxngfp_201205/include" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/platforms/paxngfp_201205/include/freetype2" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/arm-none-linux-gnueabi/libc/usr/include" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/lib/gcc/arm-none-linux-gnueabi/4.6.3/include" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


