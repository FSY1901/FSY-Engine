#include "Camera.h"

namespace FSY {

	Camera* Camera::main = nullptr;

	Camera::Camera() {

	}

	Camera::Camera(Vector3f pos, Vector3f rot, Vector3f scale) : GameObject(pos, rot, scale, "Camera"){
		if (main == nullptr)
			main = this;
	}

	Camera* Camera::GetMain() {
		return main;
	}

	void Camera::SetAsMain(Camera* cam) {
		main = cam;
	}

	void Camera::__SetViewMatrix(glm::mat4 view) {
		viewMatrix = view;
	}

	glm::mat4 Camera::_GetViewMatrix() { return viewMatrix; }

	void Camera::__SetProjectionMatrix(glm::mat4 projection) {
		projectionMatrix = projection;
	}

	glm::mat4 Camera::_GetProjectionMatrix() { return projectionMatrix; }

}