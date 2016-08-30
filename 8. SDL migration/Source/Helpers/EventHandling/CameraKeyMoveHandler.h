/*
 * CameraKeyMoveHandler.h
 *
 *  Created on: Aug 25, 2016
 *      Author: vladfedchenko
 */

#ifndef CAMERAKEYMOVEHANDLER_H_
#define CAMERAKEYMOVEHANDLER_H_

#include <map>
#include "../../Camera.h"
#include <SDL2/SDL.h>
#include "BaseEventHanler.h"
#include "../RenderHandling/BaseRenderHandler.h"

#define STD_MOVE_COEF 2.0f
#define SHIFT_MULT 5.0f

namespace VladFedchenko{
namespace GL{
namespace Helpers{
namespace EventHandling {

	class CameraKeyMoveHandler : public BaseEventHanler
		, public VladFedchenko::GL::Helpers::RenderHandling::BaseRenderHandler
	{
	private:
		Camera *camera;
		std::map<unsigned char, bool> keyPressMap;
		bool shiftFlag;

	private:
		float GetMoveCoof(unsigned long timePassed);
		void MoveCameraIfNeeded(unsigned long timePassed);

	public:
		CameraKeyMoveHandler(VladFedchenko::GL::Camera *camera);

		void HandleEvent(const SDL_Event &event);

		void BeforeFrameRender(unsigned long timeFromPrevStarted, unsigned long timeFromPrevFinished);
	};

}}}}

#endif /* CAMERAKEYMOVEHANDLER_H_ */
