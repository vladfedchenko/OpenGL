/*
 * CubeRenderObject.h
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#ifndef CUBERENDEROBJECT_H_
#define CUBERENDEROBJECT_H_

class RenderObject;
class ElementedObject;

#include "../RenderObject.h"
#include "../Camera.h"

#define SPEED 3500.0f

namespace VladFedchenko{
namespace GL{
namespace RenderObjects{

	class CubeRenderObject : public VladFedchenko::GL::RenderObject, public VladFedchenko::GL::ElementedObject
	{
	public:
		CubeRenderObject(VladFedchenko::GL::Camera *camera);

		void Render(unsigned long timeSpan);
	};

}}}

#endif //CUBERENDEROBJECT_H_
