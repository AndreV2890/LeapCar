################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../serial/Buffer.cpp \
../serial/DiViClientSeriale.cpp \
../serial/DiViSeriale.cpp \
../serial/StateTable.cpp \
../serial/Timer.cpp 

OBJS += \
./serial/Buffer.o \
./serial/DiViClientSeriale.o \
./serial/DiViSeriale.o \
./serial/StateTable.o \
./serial/Timer.o 

CPP_DEPS += \
./serial/Buffer.d \
./serial/DiViClientSeriale.d \
./serial/DiViSeriale.d \
./serial/StateTable.d \
./serial/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
serial/%.o: ../serial/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


