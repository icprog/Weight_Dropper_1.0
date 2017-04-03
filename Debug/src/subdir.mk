################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ADC.c \
../src/Buffer.c \
../src/FSM.c \
../src/I2C_controller.c \
../src/UART.c \
../src/main.c \
../src/solenoid_driver.c \
../src/status_leds.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

OBJS += \
./src/ADC.o \
./src/Buffer.o \
./src/FSM.o \
./src/I2C_controller.o \
./src/UART.o \
./src/main.o \
./src/solenoid_driver.o \
./src/status_leds.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

C_DEPS += \
./src/ADC.d \
./src/Buffer.d \
./src/FSM.d \
./src/I2C_controller.d \
./src/UART.d \
./src/main.d \
./src/solenoid_driver.d \
./src/status_leds.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32F051R8Tx -DSTM32F0 -DSTM32 -DSTM32F0DISCOVERY -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F051 -I"C:/Users/abates/workspace/Weight_Dropper_1.0/inc" -I"C:/Users/abates/workspace/Weight_Dropper_1.0/CMSIS/core" -I"C:/Users/abates/workspace/Weight_Dropper_1.0/CMSIS/device" -I"C:/Users/abates/workspace/Weight_Dropper_1.0/StdPeriph_Driver/inc" -I"C:/Users/abates/workspace/Weight_Dropper_1.0/Utilities" -I"C:/Users/abates/workspace/Weight_Dropper_1.0/RTOS/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


