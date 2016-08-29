/*
 * ClassicLMDirLightShader.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: vladfedchenko
 */

#include "ClassicLMDirLightShader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace VladFedchenko{
namespace GL{
namespace ShaderPrograms{

	ClassicLMDirLightShader::ClassicLMDirLightShader(GLuint program, Camera *camera)
		: RenderObjectShaderProgram(program), camera(camera)
	{}

	ClassicLMDirLightShader::ClassicLMDirLightShader(const ShaderInfo *shaders, int shaderCount, Camera *camera)
		: RenderObjectShaderProgram(shaders, shaderCount), camera(camera)
	{}

	void ClassicLMDirLightShader::EnableUniformLocations()
	{
		glm::vec3 toEye = glm::normalize(this->camera->GetEye() - this->camera->GetCenter());
		glm::vec3 ambient(0.1f, 0.1f, 0.1f);
		glm::vec3 lightCol(1.0f, 1.0f, 1.0f);
		glm::vec3 toLightDir(0.5, -0.5, 0.5);
		toLightDir = glm::normalize(toLightDir);
		glm::vec3 halfVector = glm::normalize(toEye + glm::normalize(toLightDir));

		float shininess = 32.0f;
		float strenght = 1.5f;

		GLuint amb_loc = glGetUniformLocation(this->program, "Ambient");
		glUniform3fv(amb_loc, 1, glm::value_ptr(ambient));

		GLuint light_col_loc = glGetUniformLocation(this->program, "LightColor");
		glUniform3fv(light_col_loc, 1, glm::value_ptr(lightCol));

		GLuint toLight_loc = glGetUniformLocation(this->program, "ToLightDir");
		glUniform3fv(toLight_loc, 1, glm::value_ptr(toLightDir));

		GLuint half_vec_loc = glGetUniformLocation(this->program, "HalfVector");
		glUniform3fv(half_vec_loc, 1, glm::value_ptr(halfVector));

		GLuint shine_loc = glGetUniformLocation(this->program, "Shininess");
		glUniform1f(shine_loc, shininess);

		GLuint str_loc = glGetUniformLocation(this->program, "ReflStrength");
		glUniform1f(str_loc, strenght);
	}

}}}


