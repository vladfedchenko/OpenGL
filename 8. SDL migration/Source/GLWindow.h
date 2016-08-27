/*
 * GLWindow.h
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#ifndef GLWINDOW_H_
#define GLWINDOW_H_

#include <SDL2/SDL.h>
#include <vector>
#include "ShaderProgram.h"
#include "Helpers/CameraKeyMoveHandler.h"
#include "Helpers/CameraMouseMoveHandler.h"
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

		std::vector<VladFedchenko::GL::RenderObjectShaderProgram*> programs;

		VladFedchenko::GL::Helpers::CameraKeyMoveHandler *cameraKeyHandler = nullptr;
		VladFedchenko::GL::Helpers::CameraMouseMoveHandler *cameraMouseHandler = nullptr;
		VladFedchenko::GL::ShaderPrograms::TextRenderProgram *textRenderer = nullptr;

		unsigned long prevFrameTime = 0;
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
		bool RemoveProgram(VladFedchenko::GL::RenderObjectShaderProgram* program);

		void RegisterCameraKeyHandler(VladFedchenko::GL::Camera *camera);
		void RegisterCameraMouseHandler(VladFedchenko::GL::Camera *camera, const glm::vec3 &rotCenter);
		void RegisterTextRenderer(const ShaderInfo *shaders, int shaderCount, std::string fontPath, int fontSize, SDL_Color textCol);

		void RenderText(int x, int y, std::string text);

		void MainLoop();
	};

}}

#endif //GLWINDOW_H_
