#ifndef FSY_TIME_H
#define FSY_TIME_H

#include "Core.h"

namespace FSY {
	
	extern class Application;

	class FSY_API Time {

	public:
		static float deltaTime();

	private:
		static float s_lastframe;

		static float s_deltaTime;

		friend class Application;

	};
};

#endif // !TIME_H
