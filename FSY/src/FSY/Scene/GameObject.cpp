#include "GameObject.h"

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

	bool GameObject::IsChild() { return m_isChild; }

	bool GameObject::CompareLast() { return position == m_LastPosition && rotation == m_lastRotation && scale == m_lastScale; }

	std::vector<GameObject*> GameObject::GetChildren() { return m_children; }

	std::vector<Component*> GameObject::__GetComponents() { return m_components; }

	glm::mat4 GameObject::GetTransformationMatrix() { return transform; }

	/////////////////////////
	//COMPONENT:
	////////////////////////

	Component::Component() {
		
	}

	void Component::SetGameObject(GameObject* g) {
		this->gameObject = g;
	}

}