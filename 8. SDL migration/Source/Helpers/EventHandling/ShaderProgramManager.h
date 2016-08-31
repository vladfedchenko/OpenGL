/*
 * ShaderProgramManager.h
 *
 *  Created on: Aug 30, 2016
 *      Author: vladfedchenko
 */

#ifndef SHADERPROGRAMMANAGER_H_
#define SHADERPROGRAMMANAGER_H_

#include "BaseEventHanler.h"
#include "../RenderHandling/BaseRenderHandler.h"
#include "../../Camera.h"
#include "../../GLWindow.h"
#include <memory>
#include "../../ShaderPrograms/ClassicLMDirLightShader.h"
#include "../../ShaderPrograms/ClassicLMPointLightShader.h"
#include "../../ShaderPrograms/ClassicLMSpotlightShader.h"
#include "../../RenderObjects/CubeRenderObject.h"
#include "../../RenderObjects/FloorRenderObject.h"

namespace VladFedchenko {
namespace GL {
namespace Helpers {
namespace EventHandling {

	class ShaderProgramManager: public BaseEventHanler,
		public RenderHandling::BaseRenderHandler
	{
	private:
		typedef enum LightModeEnum
		{
			DIRECTIONAL,
			POINT,
			SPOTLIGHT
		} LightMode;

	private:
		VladFedchenko::GL::GLWindow &window;
		LightMode curLightMode;

		std::shared_ptr<VladFedchenko::GL::ShaderPrograms::ClassicLMDirLightShader> dirLightProg;
		std::shared_ptr<VladFedchenko::GL::ShaderPrograms::ClassicLMPointLightShader> pointLightProg;
		std::shared_ptr<VladFedchenko::GL::ShaderPrograms::ClassicLMSpotlightShader> spotlightProg;

		std::shared_ptr<VladFedchenko::GL::RenderObjects::CubeRenderObject> cubeObj;
		std::shared_ptr<VladFedchenko::GL::RenderObjects::FloorRenderObject> floorObj;

		VladFedchenko::GL::RenderObjectShaderProgram *prevProg = nullptr;

	private:

		void ResetProgram(VladFedchenko::GL::RenderObjectShaderProgram *newProg);

	public:
		ShaderProgramManager(VladFedchenko::GL::GLWindow &window, VladFedchenko::GL::Camera *camera);
		~ShaderProgramManager();

		void HandleEvent(const SDL_Event &event);
		void OnFrameRendering();
	};

} /* namespace EventHandling */
} /* namespace Helpers */
} /* namespace GL */
} /* namespace VladFedchenko */

#endif /* SHADERPROGRAMMANAGER_H_ */
