################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Control_main.c \
../DcMotor.c \
../buzzer.c \
../external_eeprom.c \
../gpio.c \
../main_control.c \
../pwm.c \
../timer_1.c \
../twi.c \
../uart.c 

OBJS += \
./Control_main.o \
./DcMotor.o \
./buzzer.o \
./external_eeprom.o \
./gpio.o \
./main_control.o \
./pwm.o \
./timer_1.o \
./twi.o \
./uart.o 

C_DEPS += \
./Control_main.d \
./DcMotor.d \
./buzzer.d \
./external_eeprom.d \
./gpio.d \
./main_control.d \
./pwm.d \
./timer_1.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


