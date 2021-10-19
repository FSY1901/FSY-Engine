#include "Input.h"
#include "Application.h"

namespace FSY {

	GLFWwindow* Input::s_window = nullptr;
	int* Input::s_lastKey = nullptr;
	int* Input::s_lastButton = nullptr;

	float Input::__mx = 0;
	float Input::__my = 0;

	void Input::__SetWindow(GLFWwindow* window) {
		s_window = window;
	}

	void Input::SetCursorMode(int mode) {
		glfwSetInputMode(Application::GetInstance()->__GetWindow(), GLFW_CURSOR, mode);
	}

	bool Input::GetKey(int key) {
		if (glfwGetKey(s_window, key) == GLFW_PRESS) {

			//store the last pressed key
			s_lastKey = &key;

			return true;
		}

		return false;
	}

	bool Input::GetMouse(int button) {
		if (glfwGetMouseButton(s_window, button) == GLFW_PRESS) {

			//store the last pressed button
			s_lastButton = &button;

			return true;
		}

		return false;
	}

	bool Input::OnReleaseKey(int key) {

		if (s_lastKey != nullptr)
			if (key == *s_lastKey)
				if (glfwGetKey(s_window, key) == GLFW_RELEASE) {
					s_lastKey = nullptr;
					return true;
				}

		return false;
	}

	bool Input::OnMouseRelease(int button) {

		if (s_lastButton != nullptr)
			if (button == *s_lastButton)
				if (glfwGetMouseButton(s_window, button) == GLFW_RELEASE) {
					s_lastButton = nullptr;
					return true;
				}

		return false;

	}

	float Input::MouseX() { return __mx; }

	float Input::MouseY() { return __my; }

	int Input::WinHeight() { return Application::GetInstance()->WinHeight(); }

	int Input::WinWidth() { return Application::GetInstance()->WinWidth(); }

}