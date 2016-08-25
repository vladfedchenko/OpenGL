/*
 * CameraKeyMoveHandler.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: vladfedchenko
 */

#include "CameraKeyMoveHandler.h"
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace VladFedchenko{
namespace GL{
namespace Helpers{

	CameraKeyMoveHandler::CameraKeyMoveHandler(VladFedchenko::GL::Camera *camera)
		: camera(camera), shiftFlag(false)
	{
		this->keyPressMap['q'] = false;
		this->keyPressMap['w'] = false;
		this->keyPressMap['e'] = false;
		this->keyPressMap['a'] = false;
		this->keyPressMap['s'] = false;
		this->keyPressMap['d'] = false;
	}

	void CameraKeyMoveHandler::HandleEvent(const SDL_Event &event)
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.repeat == 0)
			{
				switch(event.key.keysym.sym)
				{
				case SDLK_a:
					this->keyPressMap['a'] = true;
					break;
				case SDLK_s:
					this->keyPressMap['s'] = true;
					break;
				case SDLK_d:
					//std::cout << "D down\n";
					this->keyPressMap['d'] = true;
					break;
				case SDLK_q:
					this->keyPressMap['q'] = true;
					break;
				case SDLK_w:
					this->keyPressMap['w'] = true;
					break;
				case SDLK_e:
					this->keyPressMap['e'] = true;
					break;
				case SDLK_LSHIFT:
					this->shiftFlag = true;
					break;
				}
			}
		}
		else if (event.type == SDL_KEYUP)
		{
			switch(event.key.keysym.sym)
			{
			case SDLK_a:
				this->keyPressMap['a'] = false;
				break;
			case SDLK_s:
				this->keyPressMap['s'] = false;
				break;
			case SDLK_d:
				//std::cout << "D up\n";
				this->keyPressMap['d'] = false;
				break;
			case SDLK_q:
				this->keyPressMap['q'] = false;
				break;
			case SDLK_w:
				this->keyPressMap['w'] = false;
				break;
			case SDLK_e:
				this->keyPressMap['e'] = false;
				break;
			case SDLK_LSHIFT:
				this->shiftFlag = false;
				break;
			}
		}
	}

	void CameraKeyMoveHandler::MoveCameraIfNeeded(unsigned long timePassed)
	{
		float moveDistCoof = this->GetMoveCoof(timePassed);

		glm::vec3 tmp;
		glm::vec3 toMove(0.0f, 0.0f, 0.0f);

		glm::vec3 center = this->camera->GetCenter();
		glm::vec3 eye = this->camera->GetEye();
		glm::vec3 up = this->camera->GetUp();

		if (keyPressMap['a'])
		{
			tmp = glm::normalize(center - eye);
			toMove += glm::normalize(glm::cross(up, tmp)) * moveDistCoof;
		}
		if (keyPressMap['d'])
		{
			tmp = glm::normalize(center - eye);
			toMove -= glm::normalize(glm::cross(up, tmp)) * moveDistCoof;
		}
		if (keyPressMap['w'])
		{
			toMove += glm::normalize(center - eye) * moveDistCoof;
		}
		if (keyPressMap['s'])
		{
			toMove -= glm::normalize(center - eye) * moveDistCoof;
		}
		if (keyPressMap['e'])
		{
			toMove += up * moveDistCoof;
		}
		if (keyPressMap['q'])
		{
			toMove -= up * moveDistCoof;
		}
		if (toMove.length() > 0)
		{
			this->camera->PositionChanged(toMove);
		}
	}

	float CameraKeyMoveHandler::GetMoveCoof(unsigned long timePassed)
	{
		float base = this->shiftFlag ? (STD_MOVE_COEF * SHIFT_MULT) : (STD_MOVE_COEF);
		return base * (float)(timePassed / 1000.0f);
	}

}}}


