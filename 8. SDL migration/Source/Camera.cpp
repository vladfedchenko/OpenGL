/*
 * Camera.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#include "Camera.h"

namespace VladFedchenko{
namespace GL{

	Camera::Camera(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up, float fov, float aspectRatio, float nearPlane, float farPlane)
		: eye(eye), center(center), up(up)
	{
		this->lookAtMatr = glm::lookAt(this->eye, this->center, this->up);
		this->projectionMatr = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
		this->ResetVPMatr();
	}

	Camera::~Camera()
	{

	}

	void Camera::ResetVPMatr()
	{
		this->vpMatr = this->projectionMatr * this->lookAtMatr;
	}

	void Camera::ResetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		this->projectionMatr = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
		this->ResetVPMatr();
	}

	void Camera::ResetView(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
	{
		this->eye = eye;
		this->center = center;
		this->up = up;

		this->lookAtMatr = glm::lookAt(this->eye, this->center, this->up);
		this->ResetVPMatr();
	}

	const glm::vec3 Camera::GetEye()
	{
		return this->eye;
	}

	const glm::vec3 Camera::GetCenter()
	{
		return this->center;
	}

	const glm::vec3 Camera::GetUp()
	{
		return this->up;
	}

	const glm::mat4 Camera::GetLookAtMatr()
	{
		return this->lookAtMatr;
	}

	const glm::mat4 Camera::GetProjectionMatr()
	{
		return this->projectionMatr;
	}

	const glm::mat4 Camera::GetVPMatr()
	{
		return this->vpMatr;
	}

}}



