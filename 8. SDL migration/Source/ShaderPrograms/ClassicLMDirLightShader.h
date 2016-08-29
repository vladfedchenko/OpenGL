/*
 * ClassicLMDirLightShader.h
 *
 *  Created on: Aug 22, 2016
 *      Author: vladfedchenko
 */

#ifndef CLASSICLMDIRLIGHTSHADER_H_
#define CLASSICLMDIRLIGHTSHADER_H_

#include "../ShaderProgram.h"

namespace VladFedchenko{
namespace GL{
namespace ShaderPrograms{

class ClassicLMDirLightShader : public RenderObjectShaderProgram
{
private:
	VladFedchenko::GL::Camera *camera;

protected:
	void EnableUniformLocations();

public:
	ClassicLMDirLightShader(const ShaderInfo *shaders, int shaderCount, Camera *camera);
	ClassicLMDirLightShader(GLuint program, Camera *camera);
};

}}}

#endif /* CLASSICLMDIRLIGHTSHADER_H_ */
