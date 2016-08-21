/*
 * ShaderProgram.h
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <vector>
#include "RenderObject.h"

namespace VladFedchenko{
namespace GL{

	typedef struct {
		GLenum type;
		const char* filename;
	} ShaderInfo;

	class ShaderProgram
	{
	private:
		GLuint program = 0;
		std::vector<VladFedchenko::GL::RenderObject*> objectsToRender;

	private:
		void ReadShader(const char* filename, std::string &s);

		virtual void EnableUniformLocations() = 0;

	public:

		ShaderProgram(const ShaderInfo *shaders, int shaderCount);
		ShaderProgram(GLuint program);
		virtual ~ShaderProgram();

		void AddObject(VladFedchenko::GL::RenderObject *object);
		bool RemoveObject(VladFedchenko::GL::RenderObject *object);

		void RenderObjects(unsigned long timeSpan);
	};

}}

#endif //SHADERPROGRAM_H_
