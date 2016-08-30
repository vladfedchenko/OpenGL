/*
 * CameraMouseMoveHandler.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: vladfedchenko
 */

#include "CameraMouseMoveHandler.h"

namespace VladFedchenko{
namespace GL{
namespace Helpers{
namespace EventHandling {

	CameraMouseMoveHandler::CameraMouseMoveHandler(VladFedchenko::GL::Camera *camera, const glm::vec3 &rotationCenter)
		: camera(camera), arcballModeActive(true), rotationCenter(rotationCenter)
	{

	}

	void CameraMouseMoveHandler::HandleEvent(const SDL_Event &event)
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.repeat == 0 && event.key.keysym.sym == SDLK_f && (event.key.keysym.mod & KMOD_CTRL) != 0)
			{
				this->arcballModeActive = !this->arcballModeActive;
			}
		}
		else if (event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON_LMASK) != 0)
		{
			glm::vec3 center = this->camera->GetCenter();
			glm::vec3 eye = this->camera->GetEye();
			glm::vec3 up = this->camera->GetUp();

			eye -= this->rotationCenter;
			center -= this->rotationCenter;

			if (event.motion.xrel != 0)
			{
				if (arcballModeActive)
				{
					this->TranslateAzimuthArcball(eye, center, up, event.motion.xrel * CAM_ROT_COEF);
				}
				else
				{
					this->TranslateHorizontalNormalCam(eye, center, up, event.motion.xrel * CAM_ROT_COEF);
				}
			}
			if (event.motion.yrel != 0)
			{
				if (arcballModeActive)
				{
					this->TranslatePolarArcball(eye, center, up, event.motion.yrel * CAM_ROT_COEF);
				}
				else
				{
					this->TranslateVerticalNormalCam(eye, center, up, event.motion.yrel * CAM_ROT_COEF);
				}
			}

			eye += this->rotationCenter;
			center += this->rotationCenter;

			this->camera->ResetView(eye, center, up);
		}
	}

	void CameraMouseMoveHandler::TranslateAzimuthArcball(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle)
	{
		glm::mat4 identity(1.0f);
		glm::mat4 rotMatr = glm::rotate(identity, angle, glm::vec3(0.0f, 0.0f, -1.0f));

		glm::vec4 homo_eye(trans_eye, 1.0);
		glm::vec4 homo_center(trans_center, 1.0);
		glm::vec4 homo_up(trans_up, 1.0);

		trans_eye = glm::vec3(rotMatr * homo_eye);
		trans_center = glm::vec3(rotMatr * homo_center);
		trans_up = glm::vec3(rotMatr * homo_up);
	}

	void CameraMouseMoveHandler::TranslatePolarArcball(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle)
	{
		glm::mat4 identity(1.0f);
		glm::vec3 right = glm::cross(trans_up, glm::normalize(trans_center - trans_eye));
		right = glm::normalize(right);

		glm::mat4 rotMatr = glm::rotate(identity, angle, right);

		glm::vec4 homo_eye(trans_eye, 1.0);
		glm::vec4 homo_center(trans_center, 1.0);
		glm::vec4 homo_up(trans_up, 1.0);

		trans_eye = glm::vec3(rotMatr * homo_eye);
		trans_center = glm::vec3(rotMatr * homo_center);
		trans_up = glm::vec3(rotMatr * homo_up);
	}

	void CameraMouseMoveHandler::TranslateHorizontalNormalCam(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle)
	{
		trans_center -= trans_eye;

		glm::mat4 identity(1.0f);
		glm::mat4 rotMatr = glm::rotate(identity, angle, glm::vec3(0.0f, 0.0f, -1.0f));

		glm::vec4 homo_center(trans_center, 1.0);
		glm::vec4 homo_up(trans_up, 1.0);

		trans_center = glm::vec3(rotMatr * homo_center);
		trans_up = glm::vec3(rotMatr * homo_up);

		trans_center += trans_eye;
	}

	void CameraMouseMoveHandler::TranslateVerticalNormalCam(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle)
	{
		glm::vec3 right = glm::cross(trans_up, glm::normalize(trans_center - trans_eye));
		right = glm::normalize(right);

		trans_center -= trans_eye;

		glm::mat4 identity(1.0f);
		glm::mat4 rotMatr = glm::rotate(identity, angle, right);

		glm::vec4 homo_center(trans_center, 1.0);
		glm::vec4 homo_up(trans_up, 1.0);

		trans_center = glm::vec3(rotMatr * homo_center);
		trans_up = glm::vec3(rotMatr * homo_up);

		trans_center += trans_eye;
	}

}}}}
