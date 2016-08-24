/*
 * FloorRenderProgram.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: vladfedchenko
 */

#include "FloorRenderObject.h"
#include "../Helpers/PNGHelper.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace VladFedchenko{
namespace GL{
namespace RenderObjects{

	FloorRenderObject::FloorRenderObject(VladFedchenko::GL::Camera *camera, std::string floorTexFile, int w, int h)
		: RenderObject((unsigned int)1, camera)
		, TexturedObject((unsigned int)1)
		, ElementedObject((unsigned int)1)
	{
		glBindVertexArray(this->vao);

		GLuint indexes[4] = {0, 1, 2, 3};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementArrays[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

		GLfloat vertices[4][3] =
		{
			{-8.0, -4.5, 0.0},
			{-8.0, 4.5, 0.0},
			{8.0, 4.5, 0.0},
			{8.0, -4.5, 0.0}
		};

		GLfloat normals[4][3] =
		{
			{0.0, 0.0, 1.0},
			{0.0, 0.0, 1.0},
			{0.0, 0.0, 1.0},
			{0.0, 0.0, 1.0}
		};

		GLfloat tex_positions[4][2] =
		{
			{0.0, 0.0},
			{0.0, 1.0},
			{1.0, 1.0},
			{1.0, 0.0}
		};

		glBindBuffer(GL_ARRAY_BUFFER, this->vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 + sizeof(GLfloat) * 12 + sizeof(GLfloat) * 8, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 12, vertices);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, sizeof(GLfloat) * 12, normals);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 2, sizeof(GLfloat) * 8, tex_positions);

		VladFedchenko::GL::Helpers::PixelColor *colors = new VladFedchenko::GL::Helpers::PixelColor[w * h];
		if (!VladFedchenko::GL::Helpers::PNGHelper::ReadPNGColors(floorTexFile, w, h, colors))
		{
			std::cerr << "Cannot create texture!\n";
			throw std::exception();
		}

		glActiveTexture(GL_TEXTURE0);
		this->FillTexture2D(this->textures[0], colors, w, h);

		delete colors;
	}

	void FloorRenderObject::Render(unsigned long timeSpan, const GLuint &parentProgram)
	{
		glBindVertexArray(this->vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementArrays[0]);

		glm::mat4 identity(1.0f);
		glm::mat4 mvp_mat = this->camera->GetVPMatr() * identity;

		GLuint mvp_matr_loc = glGetUniformLocation(parentProgram, "vertex_mvp_matr");
		glUniformMatrix4fv(mvp_matr_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textures[0]);
		GLint tex1_uniform_loc = glGetUniformLocation(parentProgram, "tex");
		glUniform1i(tex1_uniform_loc, 0);

		glBindBuffer(GL_ARRAY_BUFFER, this->vbos[0]);

		int vertex_loc = glGetAttribLocation(parentProgram, "position");
		int normal_loc = glGetAttribLocation(parentProgram, "normal");
		int tex_loc = glGetAttribLocation(parentProgram, "in_tex_coord");

		glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(vertex_loc);

		glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(GLfloat) * 12));
		glEnableVertexAttribArray(normal_loc);

		glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(GLfloat) * 12 * 2));
		glEnableVertexAttribArray(tex_loc);

		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (const void*) (0 * sizeof(GLuint)));
	}

}}}


