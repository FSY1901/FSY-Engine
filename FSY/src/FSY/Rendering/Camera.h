#ifndef CAMERA_H
#define CAMERA_H

#include "../Core/Core.h"
#include "../Scene/GameObject.h"

namespace FSY {

	FSY_API enum class CameraMode {
		Perspective,
		Orthographic
	};

	class FSY_API Camera : public GameObject {

	public:
		Camera();
		Camera(Vector3f pos, Vector3f rot, Vector3f scale);

		float fov = 45.0f;
		float zNear = 0.1f;
		float zFar = 100.0f;

		static Camera* GetMain();
		static void SetAsMain(Camera* cam);

		void __SetViewMatrix(glm::mat4 view);
		glm::mat4 _GetViewMatrix();

		void __SetProjectionMatrix(glm::mat4 projection);
		glm::mat4 _GetProjectionMatrix();

		Vector3f front;
		Vector3f up;
		Vector3f right;

		CameraMode mode = CameraMode::Perspective;

		void __SetFrustum(Frustum frustum);
		Frustum GetFrustum();

	private:
		static Camera* main;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		Frustum m_frustum;

	};

}

#endif // !CAMERA_H
