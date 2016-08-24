################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/Camera.cpp \
../Source/GLWindow.cpp \
../Source/RenderObject.cpp \
../Source/ShaderProgram.cpp 

OBJS += \
./Source/Camera.o \
./Source/GLWindow.o \
./Source/RenderObject.o \
./Source/ShaderProgram.o 

CPP_DEPS += \
./Source/Camera.d \
./Source/GLWindow.d \
./Source/RenderObject.d \
./Source/ShaderProgram.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


