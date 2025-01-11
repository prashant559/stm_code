################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ST7920_SERIAL.c \
../Src/delay.c \
../Src/main.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f1xx.c 

OBJS += \
./Src/ST7920_SERIAL.o \
./Src/delay.o \
./Src/main.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f1xx.o 

C_DEPS += \
./Src/ST7920_SERIAL.d \
./Src/delay.d \
./Src/main.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"E:/TUTORIALS/STM32/ST7920_128x64/Inc" -I"E:/TUTORIALS/STM32/ST7920_128x64/Drivers/STM32F1xx_HAL_Driver/Inc" -I"E:/TUTORIALS/STM32/ST7920_128x64/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"E:/TUTORIALS/STM32/ST7920_128x64/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"E:/TUTORIALS/STM32/ST7920_128x64/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


