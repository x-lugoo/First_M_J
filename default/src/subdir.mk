################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/base.c \
../src/dataExchange.c \
../src/display.c \
../src/handleJason.c \
../src/httppacket.c \
../src/log.c \
../src/main.c \
../src/print.c \
../src/simple_parse.c \
../src/ui_menu.c 

OBJS += \
./src/base.o \
./src/dataExchange.o \
./src/display.o \
./src/handleJason.o \
./src/httppacket.o \
./src/log.o \
./src/main.o \
./src/print.o \
./src/simple_parse.o \
./src/ui_menu.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Compiler'
	"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/bin/arm-none-linux-gnueabi-gcc" -O0 -g2 -Wall -funwind-tables -I"D:/pax/2.8.13_prolin/MiniPos/inc" -I"D:/pax/2.8.13_prolin/MiniPos/src/jansson" -I"D:/pax/2.8.13_prolin/MiniPos/src/curl" -I"D:/pax/2.8.13_prolin/MiniPos/src" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/platforms/paxngfp_201205/include" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/platforms/paxngfp_201205/include/freetype2" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/arm-none-linux-gnueabi/libc/usr/include" -I"D:/pax/pax_tool/prolin_sdk_win-2.8.13/sdk/toolchains/arm-4.4.1/lib/gcc/arm-none-linux-gnueabi/4.6.3/include" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


