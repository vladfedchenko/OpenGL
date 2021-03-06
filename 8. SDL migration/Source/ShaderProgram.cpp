/*
 * ShaderProgram.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#include "ShaderProgram.h"
#include "assert.h"
#include <string>
#include <fstream>
#include <exception>

namespace VladFedchenko{
namespace GL{

	ShaderProgram::ShaderProgram(GLuint program)
	{
		if (glIsProgram(program) != GL_TRUE)
		{
			printf("Passed ID is not a program!");
			throw std::exception();
		}
		this->program = program;
	}

	ShaderProgram::ShaderProgram(const ShaderInfo *shaders, int shaderCount)
	{
		this->program = glCreateProgram();
		if(this->program == 0)
		{
			printf("Create Program Error!");
			throw std::exception();
		}

		for (int i = 0; i < shaderCount; ++i)
		{
			GLuint shader = glCreateShader(shaders[i].type);
			std::string src;
			this->ReadShader(shaders[i].filename, src);
			const char *srcRef = src.c_str();
			glShaderSource(shader, 1, &srcRef, 0);
			glCompileShader(shader);

			GLint sStatus;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &sStatus);

			if(sStatus == GL_FALSE)
			{
				printf("Shader compile failed!");

				GLsizei infoLen;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
				char *infoLog = (char*)malloc(infoLen);
				glGetShaderInfoLog(shader, infoLen, &infoLen, infoLog);
				printf("Compile Shader:%s\n", infoLog);

				throw std::exception();
			}

			glAttachShader(this->program, shader);
		}

		glLinkProgram(this->program);

		GLint linked;
		glGetProgramiv(this->program, GL_LINK_STATUS, &linked);

		if(linked != GL_TRUE)
		{
			printf("Link failed!");

			GLsizei infoLen;
			char infoLog[100];
			glGetProgramInfoLog(program,100,&infoLen,infoLog);
			printf("Link program:%s\n",infoLog);

			throw std::exception();
		}

	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(this->program);
	}

	void ShaderProgram::ReadShader(const char* filename, std::string &s)
	{
		std::string line("");
		std::ifstream shaderFile(filename);
		if (shaderFile.is_open())
		{
			while(std::getline(shaderFile, line))
			{
				s += line + "\n";
			}
			shaderFile.close();
		}
	}

	GLuint ShaderProgram::GetProgram() const
	{
		return this->program;
	}

	//////////////////////////////////////////////////////////////////////

	RenderObjectShaderProgram::RenderObjectShaderProgram(const ShaderInfo *shaders, int shaderCount)
		: ShaderProgram(shaders, shaderCount)
	{

	}

	RenderObjectShaderProgram::RenderObjectShaderProgram(GLuint prog)
		: ShaderProgram(prog)
	{

	}



	void RenderObjectShaderProgram::AddObject(RenderObject *object)
	{
		this->objectsToRender.push_back(object);
	}

	bool RenderObjectShaderProgram::RemoveObject(RenderObject *object)
	{
		for (std::vector<RenderObject*>::iterator iter = this->objectsToRender.begin(); iter != this->objectsToRender.end(); ++iter)
		{
			if (*iter == object)
			{
				this->objectsToRender.erase(iter);
				return true;
			}
		}
		return false;
	}

	void RenderObjectShaderProgram::RenderObjects(unsigned long timeSpan)
	{
		glUseProgram(this->program);

		this->EnableUniformLocations();

		for (RenderObject* obj : this->objectsToRender)
		{
			obj->Render(timeSpan, this->program);
		}

		glUseProgram(0);
	}

}}


