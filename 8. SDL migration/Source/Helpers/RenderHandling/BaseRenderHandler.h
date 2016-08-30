/*
 * BaseRenderHandler.h
 *
 *  Created on: Aug 30, 2016
 *      Author: vladfedchenko
 */

#ifndef BASERENDERHANDLER_H_
#define BASERENDERHANDLER_H_

namespace VladFedchenko {
namespace GL {
namespace Helpers {
namespace RenderHandling {

class BaseRenderHandler {
public:
	BaseRenderHandler() { }
	virtual ~BaseRenderHandler() { }

	virtual void BeforeFrameRender(unsigned long timeFromPrevStarted, unsigned long timeFromPrevFinished) { }
	virtual void OnFrameRendering() { }
	virtual void AfterFrameRender(unsigned long lastFrameRenderTime, unsigned long timeFromPrevFinished) { }
};

} /* namespace RenderHandling */
} /* namespace Helpers */
} /* namespace GL */
} /* namespace VladFedchenko */

#endif /* BASERENDERHANDLER_H_ */
