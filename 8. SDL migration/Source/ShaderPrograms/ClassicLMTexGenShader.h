/*
 * ClassicLMTexGenShader.h
 *
 *  Created on: Aug 22, 2016
 *      Author: vladfedchenko
 */

#ifndef CLASSICLMTEXGENSHADER_H_
#define CLASSICLMTEXGENSHADER_H_

#include "../ShaderProgram.h"

namespace VladFedchenko{
namespace GL{
namespace ShaderPrograms{

class ClassicLMTexGenShader : public RenderObjectShaderProgram
{
private:
	VladFedchenko::GL::Camera *camera;

protected:
	void EnableUniformLocations();

public:
	ClassicLMTexGenShader(const ShaderInfo *shaders, int shaderCount, Camera *camera);
	ClassicLMTexGenShader(GLuint program, Camera *camera);
};

}}}

#endif /* CLASSICLMTEXGENSHADER_H_ */
