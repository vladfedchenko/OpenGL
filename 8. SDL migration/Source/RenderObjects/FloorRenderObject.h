/*
 * FloorRenderObject.h
 *
 *  Created on: Aug 21, 2016
 *      Author: vladfedchenko
 */

#ifndef FLOORRENDEROBJECT_H_
#define FLOORRENDEROBJECT_H_

#include "../RenderObject.h"

namespace VladFedchenko{
namespace GL{
namespace RenderObjects{

	class FloorRenderObject : public VladFedchenko::GL::RenderObject, public VladFedchenko::GL::TexturedObject, public VladFedchenko::GL::ElementedObject
	{
	public:
		FloorRenderObject(VladFedchenko::GL::Camera *camera, std::string floorTexFile, int w, int h);

		void Render(unsigned long timeSpan, const GLuint &parentProgram);
	};

}}}

#endif //FLOORRENDEROBJECT_H_
