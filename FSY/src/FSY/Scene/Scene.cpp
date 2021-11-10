#include "Scene.h"

namespace FSY {

	Scene::Scene() {
		m_objects.reserve(100000);
		m_meshes.reserve(10000);
		this->AddObject(&m_camera);
		this->AddObject(&m_light);
	}

	void Scene::AddObject(GameObject* obj) {
		if (m_objects.size() < c_maxObjects) {
			__CheckName(obj->name, obj);
			m_objects.push_back(obj);
		}
	}

	void Scene::AddInstanceMesh(Mesh* m) {

		if (m_meshes.size() < 10000)
			m_meshes.push_back(m);
		else
			return;

		for (auto g : m->_GetGameObjects()) {
			AddObject(g);
		}
	}

	std::string Scene::__CheckName(std::string& name, GameObject* g) {
		for (auto g1 : m_objects) {
			if (name == g1->name && g1 != g) {
				name += "+";
				return __CheckName(name, g);
			}
		}

		return name;
	}

	GameObject* Scene::GetCamera() {
		return &m_camera;
	}

	Light* Scene::GetLight() { return &m_light; }

	std::vector<GameObject*> Scene::_GetObjects() {
		return m_objects;
	}

	std::vector<Mesh*> Scene::_GetMeshes() {
		return m_meshes;
	}

}