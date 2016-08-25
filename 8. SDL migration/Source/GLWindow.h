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

#define WIDTH 854
#define HEIGHT 480

namespace VladFedchenko{
namespace GL{

	class GLWindow
	{
	private:
		SDL_Window *mainWindow;
		SDL_GLContext mainContext;

		std::vector<VladFedchenko::GL::ShaderProgram*> programs;

		VladFedchenko::GL::Helpers::CameraKeyMoveHandler *cameraKeyHandler;
		VladFedchenko::GL::Helpers::CameraMouseMoveHandler *cameraMouseHandler;

		unsigned long prevFrameTime = 0;
	private:

		bool Init();
		void CheckSDLError();
		bool SetOpenGLAttributes();

	public:
		GLWindow();
		~GLWindow();

		void AddProgram(VladFedchenko::GL::ShaderProgram* program);
		bool RemoveProgram(VladFedchenko::GL::ShaderProgram* program);

		void RegisterCameraKeyHandler(VladFedchenko::GL::Camera *camera);
		void RegisterCameraMouseHandler(VladFedchenko::GL::Camera *camera, const glm::vec3 &rotCenter);

		void MainLoop();
	};

}}

#endif //GLWINDOW_H_
