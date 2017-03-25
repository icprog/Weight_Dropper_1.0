################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32f0xx.S 

OBJS += \
./startup/startup_stm32f0xx.o 

S_UPPER_DEPS += \
./startup/startup_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32F051R8Tx -DSTM32F0 -DSTM32 -DSTM32F0DISCOVERY -DUSE_STDPERIPH_DRIVER -DSTM32F051 -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/inc" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/CMSIS/core" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/CMSIS/device" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/StdPeriph_Driver/inc" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/Utilities" -I"C:/Users/abates/workspace/Weight_Dropper_1.0_temp/RTOS/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

