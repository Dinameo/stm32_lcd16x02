################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/lcd.c \
../libs/lcd_config.c 

OBJS += \
./libs/lcd.o \
./libs/lcd_config.o 

C_DEPS += \
./libs/lcd.d \
./libs/lcd_config.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o libs/%.su libs/%.cyclo: ../libs/%.c libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/Documents/NCKH/TASK 6/lcd16x02_driver/libs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-libs

clean-libs:
	-$(RM) ./libs/lcd.cyclo ./libs/lcd.d ./libs/lcd.o ./libs/lcd.su ./libs/lcd_config.cyclo ./libs/lcd_config.d ./libs/lcd_config.o ./libs/lcd_config.su

.PHONY: clean-libs

