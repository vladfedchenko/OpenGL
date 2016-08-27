################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/ShaderPrograms/ClassicLMTexGenShader.cpp \
../Source/ShaderPrograms/ClassicLMTexLoadShader.cpp \
../Source/ShaderPrograms/TextRenderProgram.cpp 

OBJS += \
./Source/ShaderPrograms/ClassicLMTexGenShader.o \
./Source/ShaderPrograms/ClassicLMTexLoadShader.o \
./Source/ShaderPrograms/TextRenderProgram.o 

CPP_DEPS += \
./Source/ShaderPrograms/ClassicLMTexGenShader.d \
./Source/ShaderPrograms/ClassicLMTexLoadShader.d \
./Source/ShaderPrograms/TextRenderProgram.d 


# Each subdirectory must supply rules for building sources it contributes
Source/ShaderPrograms/%.o: ../Source/ShaderPrograms/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


