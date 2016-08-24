/*
 * RenderObject.h
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#ifndef RENDEROBJECT_H_
#define RENDEROBJECT_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include "Camera.h"
#include "ShaderProgram.h"
#include "Helpers/PNGHelper.h"

namespace VladFedchenko{
namespace GL{

	class ShaderProgram;

	class RenderObject
	{
	protected:
		GLuint vao = 0;
		GLuint *vbos = nullptr;
		unsigned int vboCount;

		Camera *camera = nullptr;

	public:

		RenderObject(unsigned int vboCount, Camera *camera);
		virtual ~RenderObject();

		virtual void Render(unsigned long timeSpan, const GLuint &parentProgram) = 0;

	};

	class TexturedObject
	{
	protected:
		GLuint *textures = nullptr;
		unsigned int texCount;

		virtual void FillTexture2D(GLuint &tex, VladFedchenko::GL::Helpers::PixelColor *colors, int w, int h);

	public:
		TexturedObject(unsigned int texCount);
		virtual ~TexturedObject();

	};

	class ElementedObject
	{
	protected:
		GLuint *elementArrays;
		unsigned int elementArrayCount;

	public:
		ElementedObject(unsigned int elementArrayCount);
		virtual ~ElementedObject();
	};

}}

#endif //RENDEROBJECT_H_
