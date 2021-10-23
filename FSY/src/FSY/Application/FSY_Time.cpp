#include "FSY_Time.h"

namespace FSY {

	float Time::s_lastframe = 0.0f;

	float Time::s_deltaTime = 0.0f;

	float Time::deltaTime() { return s_deltaTime; }

}