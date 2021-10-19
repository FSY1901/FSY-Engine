#ifndef SCENE_H
#define SCENE_H

#include "Core.h"
#include "Light.h"

#include <vector>
#include <string>

namespace FSY {

	class FSY_API Scene {

	public:
		Scene();

		void AddObject(GameObject* obj);
		void AddInstanceMesh(Mesh* m);

		//Engine only to check if an object with the name already exists in the Scene
		std::string __CheckName(std::string& name, int iter);

		GameObject* GetCamera();
		Light* GetLight();

		std::vector<GameObject*> _GetObjects();
		std::vector<Mesh*> _GetMeshes();

	private:
		const int c_maxObjects = 100000;
		Camera m_camera{ Vector3f(0, 0, 0), Vector3f(0, 0, -55.0f), Vector3f(1, 1, 1)};
		Light m_light{ Vector3f(0, 0, 0), Vector3f(1, 1, 1) };

		std::vector<GameObject*> m_objects = {};
		std::vector<Mesh*> m_meshes = {};

	};

}

#endif // !SCENE_H
