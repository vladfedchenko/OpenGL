/*
 * GLWindow.h
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#ifndef GLWINDOW_H_
#define GLWINDOW_H_

#include <SDL2/SDL.h>

namespace VladFedchenko{
namespace GL{

	class GLWindow
	{
	private:
		SDL_Window *mainWindow = nullptr;
		SDL_GLContext mainContext;
	};

}}

#endif //GLWINDOW_H_
