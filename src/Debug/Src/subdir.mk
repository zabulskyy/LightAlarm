################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/RTC.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/servo_car.c \
../Src/stm32f3xx_hal_msp.c \
../Src/stm32f3xx_it.c \
../Src/system_stm32f3xx.c \
../Src/tim.c \
../Src/usart.c 

OBJS += \
./Src/RTC.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/servo_car.o \
./Src/stm32f3xx_hal_msp.o \
./Src/stm32f3xx_it.o \
./Src/system_stm32f3xx.o \
./Src/tim.o \
./Src/usart.o 

C_DEPS += \
./Src/RTC.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/servo_car.d \
./Src/stm32f3xx_hal_msp.d \
./Src/stm32f3xx_it.d \
./Src/system_stm32f3xx.d \
./Src/tim.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F303xC -I"C:/Users/vzabu/Desktop/superSecretProjectNumber29-master/lightalarm/src/Inc" -I"C:/Users/vzabu/Desktop/superSecretProjectNumber29-master/lightalarm/src/Drivers/STM32F3xx_HAL_Driver/Inc" -I"C:/Users/vzabu/Desktop/superSecretProjectNumber29-master/lightalarm/src/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"C:/Users/vzabu/Desktop/superSecretProjectNumber29-master/lightalarm/src/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"C:/Users/vzabu/Desktop/superSecretProjectNumber29-master/lightalarm/src/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


