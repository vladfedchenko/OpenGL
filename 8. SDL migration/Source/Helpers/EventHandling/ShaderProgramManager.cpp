/*
 * ShaderProgramManager.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: vladfedchenko
 */

#include "ShaderProgramManager.h"

using namespace VladFedchenko::GL::RenderObjects;
using namespace VladFedchenko::GL;

namespace VladFedchenko {
namespace GL {
namespace Helpers {
namespace EventHandling {

	ShaderProgramManager::ShaderProgramManager(VladFedchenko::GL::GLWindow &window, VladFedchenko::GL::Camera *camera)
		: window(window), curLightMode(LightMode::DIRECTIONAL)
	{
		ShaderInfo shadersDirectional[] = { {GL_VERTEX_SHADER, "Source/GLSLShaders/directional.vert" },
				{GL_FRAGMENT_SHADER, "Source/GLSLShaders/directional.frag" }};

		ShaderInfo shadersPoint[] = { {GL_VERTEX_SHADER, "Source/GLSLShaders/point.vert" },
				{GL_FRAGMENT_SHADER, "Source/GLSLShaders/point.frag" }};

		ShaderInfo shadersSpotlight[] = { {GL_VERTEX_SHADER, "Source/GLSLShaders/spotlight.vert" },
				{GL_FRAGMENT_SHADER, "Source/GLSLShaders/spotlight.frag" }};

		this->dirLightProg.reset(new VladFedchenko::GL::ShaderPrograms::ClassicLMDirLightShader(shadersDirectional, 2, camera));
		this->pointLightProg.reset(new VladFedchenko::GL::ShaderPrograms::ClassicLMPointLightShader(shadersPoint, 2, camera));
		this->spotlightProg.reset(new VladFedchenko::GL::ShaderPrograms::ClassicLMSpotlightShader(shadersSpotlight, 2, camera));

		this->cubeObj.reset(new CubeRenderObject(camera));
		this->floorObj.reset(new FloorRenderObject(camera, "Source/Resources/Images/Floor.png", 1366, 768));

		this->dirLightProg->AddObject(this->cubeObj.get());
		this->dirLightProg->AddObject(this->floorObj.get());

		this->pointLightProg->AddObject(this->cubeObj.get());
		this->pointLightProg->AddObject(this->floorObj.get());

		this->spotlightProg->AddObject(this->cubeObj.get());
		this->spotlightProg->AddObject(this->floorObj.get());

		this->window.AddProgram(this->dirLightProg.get());
		this->prevProg = this->dirLightProg.get();
	}

	ShaderProgramManager::~ShaderProgramManager()
	{
	}

	void ShaderProgramManager::HandleEvent(const SDL_Event &event)
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.repeat == 0)
			{
				switch(event.key.keysym.sym)
				{
				case SDLK_1:
					if (this->curLightMode != LightMode::DIRECTIONAL)
					{
						this->ResetProgram(this->dirLightProg.get());
						this->curLightMode = LightMode::DIRECTIONAL;
					}
					break;
				case SDLK_2:
					if (this->curLightMode != LightMode::POINT)
					{
						this->ResetProgram(this->pointLightProg.get());
						this->curLightMode = LightMode::POINT;
					}
					break;
				case SDLK_3:
					if (this->curLightMode != LightMode::SPOTLIGHT)
					{
						this->ResetProgram(this->spotlightProg.get());
						this->curLightMode = LightMode::SPOTLIGHT;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	void ShaderProgramManager::ResetProgram(RenderObjectShaderProgram *newProg)
	{
		this->window.RemoveProgram(this->prevProg);
		this->window.AddProgram(newProg);
		this->prevProg = newProg;
	}

	void ShaderProgramManager::OnFrameRendering()
	{
		std::string message = "Current light mode: ";
		switch(this->curLightMode)
		{
		case LightMode::DIRECTIONAL:
			message += "Directional";
			break;
		case LightMode::POINT:
			message += "Point";
			break;
		case LightMode::SPOTLIGHT:
			message += "Spotlight";
			break;
		default:
			message += "?";
			break;
		}

		this->window.RenderText(5, 22, message);
	}

} /* namespace EventHandling */
} /* namespace Helpers */
} /* namespace GL */
} /* namespace VladFedchenko */
