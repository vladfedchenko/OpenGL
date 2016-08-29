/*
 * ClassicLMPointLightShader.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: vladfedchenko
 */

#include "ClassicLMPointLightShader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace VladFedchenko{
namespace GL{
namespace ShaderPrograms{

	ClassicLMPointLightShader::ClassicLMPointLightShader(GLuint program, Camera *camera)
		: RenderObjectShaderProgram(program), camera(camera)
	{}

	ClassicLMPointLightShader::ClassicLMPointLightShader(const ShaderInfo *shaders, int shaderCount, Camera *camera)
		: RenderObjectShaderProgram(shaders, shaderCount), camera(camera)
	{}

	void ClassicLMPointLightShader::EnableUniformLocations()
	{
		glm::vec3 toEye = glm::normalize(this->camera->GetEye() - this->camera->GetCenter());
		glm::vec3 ambient(0.1f, 0.1f, 0.1f);
		glm::vec3 lightCol(1.0f, 1.0f, 1.0f);
		glm::vec3 lightPos(0.0, 0.0, 5.0);

		float shininess = 32.0f;
		float strenght = 1.5f;
		float constantAttenuation = 0.9f;
		float linearAttenuation = 0.07f;
		float quadraticAttenuation = 1.0f - linearAttenuation - constantAttenuation;
		float amplification = 1.0f;

		GLuint amb_loc = glGetUniformLocation(this->program, "Ambient");
		glUniform3fv(amb_loc, 1, glm::value_ptr(ambient));

		GLuint light_col_loc = glGetUniformLocation(this->program, "LightColor");
		glUniform3fv(light_col_loc, 1, glm::value_ptr(lightCol));

		GLuint toLight_loc = glGetUniformLocation(this->program, "LightPosition");
		glUniform3fv(toLight_loc, 1, glm::value_ptr(lightPos));

		GLuint half_vec_loc = glGetUniformLocation(this->program, "EyeDirection");
		glUniform3fv(half_vec_loc, 1, glm::value_ptr(toEye));

		GLuint shine_loc = glGetUniformLocation(this->program, "Shininess");
		glUniform1f(shine_loc, shininess);

		GLuint str_loc = glGetUniformLocation(this->program, "ReflStrength");
		glUniform1f(str_loc, strenght);

		GLuint constantAttenuation_loc = glGetUniformLocation(this->program, "ConstantAttenuation");
		glUniform1f(constantAttenuation_loc, constantAttenuation);

		GLuint linearAttenuation_loc = glGetUniformLocation(this->program, "LinearAttenuation");
		glUniform1f(linearAttenuation_loc, linearAttenuation);

		GLuint quadraticAttenuationloc = glGetUniformLocation(this->program, "QuadraticAttenuation");
		glUniform1f(quadraticAttenuationloc, quadraticAttenuation);

		GLuint amplification_loc = glGetUniformLocation(this->program, "Amplification");
		glUniform1f(amplification_loc, amplification);
	}

}}}


