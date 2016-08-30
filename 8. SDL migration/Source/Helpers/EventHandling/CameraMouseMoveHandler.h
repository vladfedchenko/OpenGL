/*
 * CameraMouseMoveHandler.h
 *
 *  Created on: Aug 25, 2016
 *      Author: vladfedchenko
 */

#ifndef CAMERAMOUSEMOVEHANDLER_H_
#define CAMERAMOUSEMOVEHANDLER_H_

#include "../../Camera.h"
#include <SDL2/SDL.h>
#include "BaseEventHanler.h"

#define CAM_ROT_COEF 0.36f

namespace VladFedchenko{
namespace GL{
namespace Helpers{
namespace EventHandling {

	class CameraMouseMoveHandler : public BaseEventHanler
	{
	private:
		VladFedchenko::GL::Camera *camera;
		bool arcballModeActive;
		glm::vec3 rotationCenter;

	public:
		CameraMouseMoveHandler(VladFedchenko::GL::Camera *camera, const glm::vec3 &rotationCenter);

		void HandleEvent(const SDL_Event &event);

		void TranslateAzimuthArcball(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle);
		void TranslatePolarArcball(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle);
		void TranslateHorizontalNormalCam(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle);
		void TranslateVerticalNormalCam(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle);
	};

}}}}

#endif /* CAMERAMOUSEMOVEHANDLER_H_ */
