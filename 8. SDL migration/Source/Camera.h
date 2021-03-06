/*
 * Camera.h
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace VladFedchenko{
namespace GL{

	class Camera
	{
	private:
		glm::vec3 eye;
		glm::vec3 center;
		glm::vec3 up;

		glm::mat4 lookAtMatr;
		glm::mat4 projectionMatr;
		glm::mat4 vpMatr;

	private:
		void ResetVPMatr();

	public:
		Camera(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up, float fov, float aspectRation, float nearPlane, float farPlane);
		~Camera();

		void ResetProjection(float fov, float aspectRation, float nearPlane, float farPlane);
		void ResetView(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);

		const glm::vec3 GetEye();
		const glm::vec3 GetCenter();
		const glm::vec3 GetUp();

		const glm::mat4 GetLookAtMatr();
		const glm::mat4 GetProjectionMatr();
		const glm::mat4 GetVPMatr();

		void PositionChanged(const glm::vec3 &toMove);
	};

}}

#endif //CAMERA_H_
