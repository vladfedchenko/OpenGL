/*
 * GLWindow.h
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#ifndef GLWINDOW_H_
#define GLWINDOW_H_

#include <SDL2/SDL.h>
#include <list>
#include "ShaderProgram.h"
#include "Helpers/EventHandling/BaseEventHanler.h"
#include "Helpers/EventHandling/CameraKeyMoveHandler.h"
#include "Helpers/EventHandling/CameraMouseMoveHandler.h"
#include "Helpers/RenderHandling/BaseRenderHandler.h"
#include "Camera.h"
#include <SDL2/SDL_ttf.h>
#include "ShaderPrograms/TextRenderProgram.h"

#define WIDTH 854
#define HEIGHT 480

#define FPS_RENEW_CYCLE 1000.0f //in miliseconds

namespace VladFedchenko{
namespace GL{

	class GLWindow
	{
	private:
		SDL_Window *mainWindow;
		SDL_GLContext mainContext;

		std::list<VladFedchenko::GL::RenderObjectShaderProgram*> programs;
		std::list<VladFedchenko::GL::Helpers::EventHandling::BaseEventHanler*> eventHandlers;
		std::list<VladFedchenko::GL::Helpers::RenderHandling::BaseRenderHandler*> renderHandlers;
		VladFedchenko::GL::ShaderPrograms::TextRenderProgram *textRenderer = nullptr;

		unsigned long prevFrameFinishedTime = 0;
		unsigned long prevFrameStartedTime = 0;
		unsigned long fpsTimer = 0;
		float fps = 0.0f;
		unsigned int fpcChechCount = 0;

	private:

		bool Init();
		void CheckSDLError();
		bool SetOpenGLAttributes();

	public:
		GLWindow();
		~GLWindow();

		void AddProgram(VladFedchenko::GL::RenderObjectShaderProgram* program);
		void RemoveProgram(VladFedchenko::GL::RenderObjectShaderProgram* program);

		void RegisterEventHandler(VladFedchenko::GL::Helpers::EventHandling::BaseEventHanler* handler);
		void UnregisterEventHandler(VladFedchenko::GL::Helpers::EventHandling::BaseEventHanler* handler);

		void RegisterRenderHandler(VladFedchenko::GL::Helpers::RenderHandling::BaseRenderHandler* handler);
		void UnregisterRenderHandler(VladFedchenko::GL::Helpers::RenderHandling::BaseRenderHandler* handler);

		void RegisterTextRenderer(const ShaderInfo *shaders, int shaderCount, std::string fontPath, int fontSize, SDL_Color textCol);

		void RenderText(int x, int y, std::string text);

		void MainLoop();
	};

}}

#endif //GLWINDOW_H_
