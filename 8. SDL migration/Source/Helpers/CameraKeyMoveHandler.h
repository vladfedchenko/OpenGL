/*
 * CameraKeyMoveHandler.h
 *
 *  Created on: Aug 25, 2016
 *      Author: vladfedchenko
 */

#ifndef CAMERAKEYMOVEHANDLER_H_
#define CAMERAKEYMOVEHANDLER_H_

#include <map>
#include "../Camera.h"
#include <SDL2/SDL.h>

#define STD_MOVE_COEF 2.0f
#define SHIFT_MULT 5.0f

namespace VladFedchenko{
namespace GL{
namespace Helpers{

	class CameraKeyMoveHandler
	{
	private:
		Camera *camera;
		std::map<unsigned char, bool> keyPressMap;
		bool shiftFlag;

	private:
		float GetMoveCoof(unsigned long timePassed);

	public:
		CameraKeyMoveHandler(VladFedchenko::GL::Camera *camera);

		void HandleEvent(const SDL_Event &event);
		void MoveCameraIfNeeded(unsigned long timePassed);


	};

}}}

#endif /* CAMERAKEYMOVEHANDLER_H_ */
