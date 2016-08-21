/*
 * CubeRenderObject.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#include "CubeRenderObject.h"
#include "../RenderObject.h"
#include "../Camera.h"
#include <cmath>

using namespace VladFedchenko::GL;

namespace VladFedchenko{
namespace GL{
namespace RenderObjects{

	CubeRenderObject::CubeRenderObject(Camera *camera)
		: RenderObject((unsigned int)6, camera)
		, ElementedObject((unsigned int)1)
	{
		glBindVertexArray(this->vao);

		GLuint indexes[4] = {0, 1, 2, 3};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementArrays[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

		GLfloat vertices[24][3] =
		{
			{-1.0f, -1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, -1.0f, 1.0f},
			{-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
			{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, -1.0f},
			{-1.0f, 1.0f, -1.0f}, {-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, -1.0f},
			{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, -1.0f},
		};

		GLfloat normals[24][3] =
		{
			{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
			{0.0, 0.0, -1.0}, {0.0, 0.0, -1.0}, {0.0, 0.0, -1.0}, {0.0, 0.0, -1.0},
			{0.0, -1.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, -1.0, 0.0},
			{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
			{-1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0},
			{1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}
		};

		GLfloat tex_positions[4][2] = { {0.0, 0.0}
			, {0.0, 1.0}
			, {1.0, 1.0}
			, {1.0, 0.0}};

		for (int i = 0; i < 6; ++i)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->vbos[i]);

			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 + sizeof(GLfloat) * 12 + sizeof(GLfloat) * 8, NULL, GL_STATIC_DRAW);

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 12, &vertices[i * 4]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, sizeof(GLfloat) * 12, &normals[i * 4]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 2, sizeof(GLfloat) * 8, tex_positions);
		}
	}

	void CubeRenderObject::Render(unsigned long timeSpan)
	{
		glBindVertexArray(this->vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementArrays[0]);

		float translaionCoof = std::sin(timeSpan / SPEED * M_PI / 2.0) * 7;
		float rotateAngle = M_PI * timeSpan / SPEED / 2.0;

		glm::mat4 identity(1.0f);
		glm::mat3 iden3(1.0f);
		glm::mat4 model_matr = glm::translate(identity, glm::vec3(translaionCoof, 0.0f, 2.0f));

		glm::mat4 rotate_matr = glm::rotate(identity, rotateAngle * 360.0f / (float)M_PI, glm::vec3(0.0, 0.0, 1.0));
		glm::mat3 rotate3(rotate_matr);

		glm::mat4 mvp_mat = this->camera->GetVPMatr() * model_matr * rotate_matr;

//		GLuint normal_transform_matr_loc = glGetUniformLocation(cubeProgram, "normal_transform_matr");
//		glUniformMatrix3fv(normal_transform_matr_loc, 1, GL_FALSE, glm::value_ptr(rotate3));
//
//		GLuint vertex_mvp_matr_loc = glGetUniformLocation(cubeProgram, "vertex_mvp_matr");
//		glUniformMatrix4fv(vertex_mvp_matr_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));
	}

}}}

