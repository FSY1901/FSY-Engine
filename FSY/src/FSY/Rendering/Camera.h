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

	private:
		static Camera* main;

	};

}

#endif // !CAMERA_H
