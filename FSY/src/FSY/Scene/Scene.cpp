#include "Scene.h"

namespace FSY {

	Scene* Scene::activeScene = nullptr;

	Scene::Scene() {
		m_objects.reserve(100000);
		m_meshes.reserve(10000);
		m_camera.rotation = Vector3f(0, 0, 0);
		this->AddObject(&m_camera);
		this->AddObject(&m_light);
	}

	void Scene::AddObject(GameObject* obj) {
		if (m_objects.size() < c_maxObjects) {
			__CheckName(obj->name, obj);
			m_objects.push_back(obj);
		}
	}

	void Scene::DeleteObject(GameObject* obj) {
		for (int i = 0; i < m_objects.size(); i++) {
			GameObject* g = m_objects[i];
			if (obj == g) {
				m_objects.erase(m_objects.begin() + i);
				RemoveChildrenOfObject(obj);
				if (obj->IsChild()) {
					obj->GetParent()->RemoveChild(obj);
				}
				if (obj->HasMesh()) {
					Mesh* m = obj->GetMesh();
					m->RemoveGameObject(obj);
				}
			}
		}
	}

	void Scene::RemoveChildrenOfObject(GameObject* parent) {
		if (parent->GetChildren().size() != 0) {
			for (int i = 0; i < parent->GetChildren().size(); i++) {
				GameObject* g1 = parent->GetChildren()[i];
				for (int j = 0; j < m_objects.size(); j++) {
					if (g1 == m_objects[j]) {
						RemoveChildrenOfObject(g1);
						m_objects.erase(m_objects.begin() + j);
						if (g1->HasMesh()) {
							g1->GetMesh()->RemoveGameObject(g1);
						}
					}
				}
			}
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

	Camera* Scene::GetCamera() {
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