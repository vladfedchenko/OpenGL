################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/RenderObjects/CubeRenderObject.cpp \
../Source/RenderObjects/FloorRenderObject.cpp 

OBJS += \
./Source/RenderObjects/CubeRenderObject.o \
./Source/RenderObjects/FloorRenderObject.o 

CPP_DEPS += \
./Source/RenderObjects/CubeRenderObject.d \
./Source/RenderObjects/FloorRenderObject.d 


# Each subdirectory must supply rules for building sources it contributes
Source/RenderObjects/%.o: ../Source/RenderObjects/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


