#ifndef SCENE_H
#define SCENE_H

#include "../Core/Core.h"
#include "Light.h"

#include <vector>
#include <string>

namespace FSY {

	enum class FSY_API SceneState {
		Edit = 0, Play = 1
	};

	class FSY_API Scene {

	public:
		Scene();

		void AddObject(GameObject* obj);
		void DeleteObject(GameObject* obj);
		void AddInstanceMesh(Mesh* m);

		//Engine only to check if an object with the name already exists in the Scene
		std::string __CheckName(std::string& name, GameObject* g);

		Camera* GetCamera();
		Light* GetLight();

		std::vector<GameObject*> _GetObjects();
		std::vector<Mesh*> _GetMeshes();

		SceneState state;

	private:
		const int c_maxObjects = 100000;
		Camera m_camera{ Vector3f(0, 0, 0), Vector3f(0, -90.0f, 0), Vector3f(1, 1, 1)};
		Light m_light{ Vector3f(0, 0, 0), Vector3f(1, 1, 1) , Vector3f(0.2f, 0.2f, 0.2f), Vector3f(0.2f, 0.2f, 0.2f) };

		std::vector<GameObject*> m_objects = {};
		std::vector<Mesh*> m_meshes = {};

		//Used when deleting an Object from the Scene
		void RemoveChildrenOfObject(GameObject* parent);

	};

}

#endif // !SCENE_H
