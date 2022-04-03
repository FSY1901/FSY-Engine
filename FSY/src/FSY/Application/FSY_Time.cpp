#include "FSY_Time.h"

#include "../../vendor/OpenGL/include/glfw3.h"

namespace FSY {

	float Time::s_lastframe = 0.0f;

	float Time::s_deltaTime = 0.0f;

	float Time::deltaTime() { return s_deltaTime; }

	float Time::currentTime() { return glfwGetTime(); };

}