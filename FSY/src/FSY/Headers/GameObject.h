#include "Core.h"
#include "Mesh.h"
#include "../Maths/Maths.h"

#include <vector>
#include <Windows.h>
#include <string>
#include <typeinfo>

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "../../vendor/imgui/imgui_stdlib.h"


using namespace FSY::MATHS;

namespace FSY {

	class FSY_API Component;

	class FSY_API GameObject {

	public:
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;
		std::string name = "";

		GameObject();
		GameObject(Vector3f pos, Vector3f rot, Vector3f scale, const char* Name);

		void AddChild(GameObject* g);
		//ENGINE ONLY --> DONT USE
		void __UpdateChildren();

		/// <summary>
		/// Returns true if the GameObject has a Mesh.
		/// </summary>
		bool HasMesh();
		bool IsChild();

		std::vector<GameObject*> GetChildren();

#pragma region Component Stuff

		//Should be used by the Engine only
		std::vector<Component*> __GetComponents();

		/// <summary>
		/// Adds a Component to the GameObject.
		/// Components add behaviour to a GameObject.
		/// </summary>
		/// <typeparam name="T">The Type of Component to add</typeparam>
		template<class T> T* AddComponent() {
			Component *c = new T();
			c->SetGameObject(this);
			m_components.push_back(c);
			return (T*)c;
		}

		/// <summary>
		/// Removes the given Component from the GameObject.
		/// The Component won't be able to control the GameObject anymore.
		/// </summary>
		/// <typeparam name="T">The type of Component to remove</typeparam>
		template<class T> void RemoveComponent() {
			Component* c = new T();
			for (auto& comp : m_components) {
				int index = 0;
				if (comp->getID() == c->getID()) {
					m_components.erase(m_components.begin() + index);
					delete c;
					return;
				}
				index++;
			}
		}

		/// <summary>
		/// Checks if the GameObject has the given Component on it, if so, this function
		/// returns the Component which matches the given type.
		/// </summary>
		/// <typeparam name="T">The Component to receive.</typeparam>
		template<class T> T* GetComponent() {
			for (int i = 0; i < m_components.size(); ++i) {
				if (typeid(T) == typeid(*m_components[i])) {
					return (T*)m_components[i];
				}
			}
		}

		template<class T> bool HasComponent() {
			Component* c = new T();
			const char* ID = c->getID();
			delete c;
			for (int i = 0; i < m_components.size(); ++i) {
				c = m_components[i];
				if (ID == c->getID()) {
					return true;
				}
			}

			return false;
		}

#pragma endregion


	private:
		bool m_hasMesh = false;
		bool m_isChild = false;
		Vector3f m_LastPosition;
		Vector3f m_lastRotation;
		Vector3f m_lastScale;

		std::vector<Component*> m_components;
		std::vector<GameObject*> m_children;

	};

	class FSY_API Component {

	public:
		Component();
		/// <summary>
		/// Gets called on the first frame.
		/// </summary>
		virtual void Start() {}
		/// <summary>
		/// Gets called every frame.
		/// </summary>
		virtual void Update() {}

		void SetGameObject(GameObject* g);

		const char* getID();
		//Should return the way the Component is displayed in the Inspector.
		virtual const char* getName() = 0;
		//Should use ImGui for an easy way to change values
		virtual void DrawUI() = 0;

	protected:
		GameObject* gameObject;
	private:
		static const char* m_ID;
		UUID m_UID;

	};

}