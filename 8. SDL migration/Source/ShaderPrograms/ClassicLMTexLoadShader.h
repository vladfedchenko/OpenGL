/*
 * ClassicLMTexLoadShader.h
 *
 *  Created on: Aug 23, 2016
 *      Author: vladfedchenko
 */

#ifndef CLASSICLMTEXLOADSHADER_H_
#define CLASSICLMTEXLOADSHADER_H_

#include "../ShaderProgram.h"

namespace VladFedchenko{
namespace GL{
namespace ShaderPrograms{


class ClassicLMTexLoadShader : public RenderObjectShaderProgram
{
private:
	VladFedchenko::GL::Camera *camera;

protected:
	void EnableUniformLocations();

public:
	ClassicLMTexLoadShader(const ShaderInfo *shaders, int shaderCount, Camera *camera);
	ClassicLMTexLoadShader(GLuint program, Camera *camera);

	void AddObject(VladFedchenko::GL::RenderObject *object);
};

}}}

#endif /* CLASSICLMTEXLOADSHADER_H_ */
