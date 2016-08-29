/*
 * ClassicLMPointLightShader.h
 *
 *  Created on: Aug 22, 2016
 *      Author: vladfedchenko
 */

#ifndef CLASSICLMPOINTLIGHTSHADER_H_
#define CLASSICLMPOINTLIGHTSHADER_H_

#include "../ShaderProgram.h"

namespace VladFedchenko{
namespace GL{
namespace ShaderPrograms{

class ClassicLMPointLightShader : public RenderObjectShaderProgram
{
private:
	VladFedchenko::GL::Camera *camera;

protected:
	void EnableUniformLocations();

public:
	ClassicLMPointLightShader(const ShaderInfo *shaders, int shaderCount, Camera *camera);
	ClassicLMPointLightShader(GLuint program, Camera *camera);
};

}}}

#endif /* CLASSICLMPOINTLIGHTSHADER_H_ */
