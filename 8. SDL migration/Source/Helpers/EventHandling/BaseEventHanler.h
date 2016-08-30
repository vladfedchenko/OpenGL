/*
 * BaseEventHanler.h
 *
 *  Created on: Aug 30, 2016
 *      Author: vladfedchenko
 */

#ifndef BASEEVENTHANLER_H_
#define BASEEVENTHANLER_H_

#include <SDL2/SDL.h>

namespace VladFedchenko {
namespace GL {
namespace Helpers {
namespace EventHandling {

	class BaseEventHanler {
	public:
		inline BaseEventHanler() { }
		virtual inline ~BaseEventHanler() { }

		virtual void HandleEvent(const SDL_Event &event) = 0;
	};

} /* namespace EventHandling */
} /* namespace Helpers */
} /* namespace GL */
} /* namespace VladFedchenko */

#endif /* BASEEVENTHANLER_H_ */
