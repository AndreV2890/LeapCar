################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../threads_managing/my_thread_lib.c 

OBJS += \
./threads_managing/my_thread_lib.o 

C_DEPS += \
./threads_managing/my_thread_lib.d 


# Each subdirectory must supply rules for building sources it contributes
threads_managing/%.o: ../threads_managing/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


