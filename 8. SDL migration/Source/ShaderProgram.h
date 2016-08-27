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

	class RenderObject;

	typedef struct {
		GLenum type;
		const char* filename;
	} ShaderInfo;

	class ShaderProgram
	{

	private:
		void ReadShader(const char* filename, std::string &s);

	protected:
		GLuint program;
		virtual void EnableUniformLocations() = 0;

	public:
		ShaderProgram(const ShaderInfo *shaders, int shaderCount);
		ShaderProgram(GLuint program);
		virtual ~ShaderProgram();

		GLuint GetProgram() const;
	};

	class RenderObjectShaderProgram : public ShaderProgram
	{
	protected:
		std::vector<VladFedchenko::GL::RenderObject*> objectsToRender;

	public:
		RenderObjectShaderProgram(const ShaderInfo *shaders, int shaderCount);
		RenderObjectShaderProgram(GLuint program);

		virtual void AddObject(VladFedchenko::GL::RenderObject *object);
		bool RemoveObject(VladFedchenko::GL::RenderObject *object);

		virtual void RenderObjects(unsigned long timeSpan);

	};

}}

#endif //SHADERPROGRAM_H_
