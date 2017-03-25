################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ADC.c \
../src/Buffer.c \
../src/I2C_controller.c \
../src/UART_Receiver.c \
../src/main.c \
../src/status_leds.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

OBJS += \
./src/ADC.o \
./src/Buffer.o \
./src/I2C_controller.o \
./src/UART_Receiver.o \
./src/main.o \
./src/status_leds.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

C_DEPS += \
./src/ADC.d \
./src/Buffer.d \
./src/I2C_controller.d \
./src/UART_Receiver.d \
./src/main.d \
./src/status_leds.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32F051R8Tx -DSTM32F0 -DSTM32 -DSTM32F0DISCOVERY -DUSE_STDPERIPH_DRIVER -DSTM32F051 -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/inc" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/CMSIS/core" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/CMSIS/device" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/StdPeriph_Driver/inc" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/Utilities" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/RTOS/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


