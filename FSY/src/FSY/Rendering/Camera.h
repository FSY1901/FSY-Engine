#ifndef CAMERA_H
#define CAMERA_H

#include "../Core/Core.h"
#include "../Scene/GameObject.h"

namespace FSY {

	class FSY_API Camera : public GameObject {

	public:
		Camera();
		Camera(Vector3f pos, Vector3f rot, Vector3f scale);

		static Camera* GetMain();
		static void SetAsMain(Camera* cam);

		void __SetViewMatrix(glm::mat4 view);
		glm::mat4 _GetViewMatrix();

		void __SetProjectionMatrix(glm::mat4 projection);
		glm::mat4 _GetProjectionMatrix();

	private:
		static Camera* main;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

	};

}

#endif // !CAMERA_H
