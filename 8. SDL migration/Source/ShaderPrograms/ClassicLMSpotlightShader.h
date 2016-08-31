/*
 * ClassicLMSpotlightShader.h
 *
 *  Created on: Aug 31, 2016
 *      Author: vladfedchenko
 */

#ifndef CLASSICLMSPOTLIGHTSHADER_H_
#define CLASSICLMSPOTLIGHTSHADER_H_

#include "../ShaderProgram.h"

namespace VladFedchenko {
namespace GL {
namespace ShaderPrograms {

class ClassicLMSpotlightShader: public RenderObjectShaderProgram
{
private:
	VladFedchenko::GL::Camera *camera;

protected:
	void EnableUniformLocations();

public:
	ClassicLMSpotlightShader(const ShaderInfo *shaders, int shaderCount, Camera *camera);
	ClassicLMSpotlightShader(GLuint program, Camera *camera);
};

} /* namespace ShaderPrograms */
} /* namespace GL */
} /* namespace VladFedchenko */

#endif /* CLASSICLMSPOTLIGHTSHADER_H_ */
