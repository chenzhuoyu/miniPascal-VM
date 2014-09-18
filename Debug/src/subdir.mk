################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/callstack.c \
../src/instr.c \
../src/io.c \
../src/main.c \
../src/mempool.c \
../src/variable.c \
../src/varpool.c \
../src/varstack.c 

OBJS += \
./src/callstack.o \
./src/instr.o \
./src/io.o \
./src/main.o \
./src/mempool.o \
./src/variable.o \
./src/varpool.o \
./src/varstack.o 

C_DEPS += \
./src/callstack.d \
./src/instr.d \
./src/io.d \
./src/main.d \
./src/mempool.d \
./src/variable.d \
./src/varpool.d \
./src/varstack.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/desperedo/Workspaces/Miscellaneous/miniPascal VM/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu99 -Wno-unused-result -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


