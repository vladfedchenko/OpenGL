################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/Helpers/EventHandling/CameraKeyMoveHandler.cpp \
../Source/Helpers/EventHandling/CameraMouseMoveHandler.cpp \
../Source/Helpers/EventHandling/ShaderProgramManager.cpp 

OBJS += \
./Source/Helpers/EventHandling/CameraKeyMoveHandler.o \
./Source/Helpers/EventHandling/CameraMouseMoveHandler.o \
./Source/Helpers/EventHandling/ShaderProgramManager.o 

CPP_DEPS += \
./Source/Helpers/EventHandling/CameraKeyMoveHandler.d \
./Source/Helpers/EventHandling/CameraMouseMoveHandler.d \
./Source/Helpers/EventHandling/ShaderProgramManager.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Helpers/EventHandling/%.o: ../Source/Helpers/EventHandling/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


