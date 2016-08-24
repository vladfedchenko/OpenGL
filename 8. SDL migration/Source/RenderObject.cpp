/*
 * RenderObject.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#include "RenderObject.h"

namespace VladFedchenko{
namespace GL{

// RenderObject region

	RenderObject::RenderObject(unsigned int vboCount, Camera *camera)
		: vboCount(vboCount), camera(camera)
	{
		this->vbos= new GLuint[this->vboCount];
		glGenVertexArrays(1, &this->vao);
		glGenBuffers(this->vboCount, this->vbos);
	}

	RenderObject::~RenderObject()
	{
		this->camera = nullptr;

		glDeleteBuffers(this->vboCount, this->vbos);
		glDeleteVertexArrays(1, &this->vao);

		delete this->vbos;
	}

// end RenderObject region

// TexturedObject region

	TexturedObject::TexturedObject(unsigned int texCount)
		: texCount(texCount)
	{
		this->textures = new GLuint[this->texCount];
		glGenTextures(this->texCount, this->textures);
	}

	TexturedObject::~TexturedObject()
	{
		glDeleteTextures(this->texCount, this->textures);
		delete this->textures;
	}

	void TexturedObject::FillTexture2D(GLuint &tex, VladFedchenko::GL::Helpers::PixelColor *colors, int w, int h)
	{
		//glActiveTexture(activeTex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); //base mipmap level
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0); //max mipmap levels
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

// end TexturedObject region

// ElementedObject region

	ElementedObject::ElementedObject(unsigned int elementArrayCount)
		: elementArrayCount(elementArrayCount)
	{
		this->elementArrays = new GLuint[this->elementArrayCount];
		glGenBuffers(this->elementArrayCount, this->elementArrays);
	}

	ElementedObject::~ElementedObject()
	{
		glDeleteBuffers(this->elementArrayCount, this->elementArrays);
		delete this->elementArrays;
	}

// end ElementedObject region

}}

