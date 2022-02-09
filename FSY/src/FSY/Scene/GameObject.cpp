#include "GameObject.h"
#include "Scene.h"

namespace FSY {

	GameObject::GameObject() {

	}

	GameObject::GameObject(Vector3f pos, Vector3f rot, Vector3f scale, const char* Name) 
		: position(pos), rotation(rot), scale(scale), name(Name), m_LastPosition(pos), m_lastRotation(rot), m_lastScale(scale)
	{
		m_components.reserve(1000);
		m_children.reserve(1000);
	}

	void GameObject::AddChild(GameObject* g) {
		g->m_isChild = true;
		m_children.push_back(g);
	}

	void GameObject::__UpdateChildren() {
		if (m_children.size() > 0) {
			if (m_LastPosition != position) {
				Vector3f offset = position - m_LastPosition;
				for (auto g : m_children) {
					g->position += offset;
				}
			}
			if (m_lastRotation != rotation) {
				Vector3f offset = rotation - m_lastRotation;
				for (auto g : m_children) {
					g->rotation += offset;
				}
			}
			if (m_lastScale != scale) {
				Vector3f offset = scale - m_lastScale;
				for (auto g : m_children) {
					g->scale += offset;
				}
			}
		}
		m_LastPosition = position;
		m_lastRotation = rotation;
		m_lastScale = scale;
	}

	bool GameObject::HasMesh(){ return m_hasMesh; }

	Mesh* GameObject::GetMesh() { return m_mesh; }

	bool GameObject::IsChild() { return m_isChild; }

	bool GameObject::CompareLast() { return position == m_LastPosition && rotation == m_lastRotation && scale == m_lastScale; }

	std::vector<GameObject*> GameObject::GetChildren() { return m_children; }

	std::vector<Component*> GameObject::__GetComponents() { return m_components; }

	glm::mat4 GameObject::GetTransformationMatrix() { return m_transform; }

	Sphere GameObject::GetBoundingSphere() { return m_boundingSphere; }

	void GameObject::CreateGameObject(bool asChild, GameObject* parent, Scene* scene) {
		GameObject* g;
		if (parent != nullptr)
			g = new GameObject(parent->position, Vector3f(0, 0, 0), Vector3f(1, 1, 1), "New Object");
		else
			g = new GameObject(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1), "New Object");

		if (asChild) {
			parent->AddChild(g);
			scene->AddObject(g);
		}
		scene->AddObject(g);
	}

	/////////////////////////
	//COMPONENT:
	////////////////////////

	Component::Component() {
		
	}

	void Component::SetGameObject(GameObject* g) {
		this->gameObject = g;
	}

}