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
