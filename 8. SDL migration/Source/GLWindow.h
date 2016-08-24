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

		bool Init();
		void CheckSDLError();
		bool SetOpenGLAttributes();

	public:
		GLWindow();
		~GLWindow();

		void AddProgram(VladFedchenko::GL::ShaderProgram* program);
		bool RemoveProgram(VladFedchenko::GL::ShaderProgram* program);

		void MainLoop();
	};

}}

#endif //GLWINDOW_H_
