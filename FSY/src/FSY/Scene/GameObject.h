#include "../Core/Core.h"
#include "Mesh.h"
#include "../Maths/Maths.h"
#include "../Rendering/FrustumCulling.h"
#include "../Editor/Console.h"

#include <vector>
#include <string>


#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

namespace FSY {

	class FSY_API Component;

	extern class Application;
	extern class Scene;

	class FSY_API GameObject {

	public:
		Vector3f position;
		Quaternion rotation;
		Vector3f scale;
		std::string name = "";

		GameObject();
		GameObject(Vector3f pos, Vector3f rot, Vector3f scale, const char* Name);

		void AddChild(GameObject* g);
		void RemoveChild(GameObject* g);
		//ENGINE ONLY --> DONT USE
		void __UpdateChildren();

		/// <summary>
		/// Returns true if the GameObject has a Mesh.
		/// </summary>
		bool HasMesh();
		Mesh* GetMesh();
		/// <summary>
		/// Returns true if the GameObject is a Child of another GameObject
		/// </summary>
		/// <returns></returns>
		bool IsChild();

		//Returns the parent GameObject if this GameObject is a child
		GameObject* GetParent();

		std::vector<GameObject*> GetChildren();

		glm::mat4 GetTransformationMatrix();

		Sphere GetBoundingSphere();

#pragma region Component Stuff

		//Should be used by the Engine only
		std::vector<Component*> __GetComponents();

		/// <summary>
		/// Adds a Component to the GameObject.
		/// Components add behaviour to a GameObject.
		/// </summary>
		/// <typeparam name="T">The Type of Component to add</typeparam>
		template<class T> T* AddComponent() {
			Component* c = new T();
			if (this->HasComponent<T>()) {
				Console::Warn( name + " already has a Component of type " + "\'" + c->getName() + "\'" + " attached!");
				delete c;
				return GetComponent<T>();
			}
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
				if (comp->getName() == c->getName()) {
					m_components.erase(m_components.begin() + index);
					delete c;
					return;
				}
				index++;
			}
			Console::Warn(name + " doesn't have a Component of type " + "\'" + c->getName() + "\'" + " attached!");
			delete c;
		}

		/// <summary>
		/// Checks if the GameObject has the given Component on it, if so, this function
		/// returns the Component which matches the given type.
		/// </summary>
		/// <typeparam name="T">The Component to receive.</typeparam>
		template<class T> T* GetComponent() {
			Component* c = new T();
			for (auto comp : m_components) {
				if (c->getName() == comp->getName()) {
					return (T*)comp;
				}
			}
			Console::Warn(name + " doesn't have a Component of type " + "\'" + c->getName() + "\'" + " attached!");
			delete c;
		}

		template<class T> bool HasComponent() {
			Component* c = new T();
			const char* ID = c->getName();
			delete c;
			for (int i = 0; i < m_components.size(); ++i) {
				c = m_components[i];
				if (ID == c->getName()) {
					return true;
				}
			}

			return false;
		}

#pragma endregion

		static void CreateGameObject(bool asChild, GameObject* parent, Scene* scene);

	private:
		bool m_hasMesh = false;
		bool m_isChild = false;

		bool CompareLast();

		Vector3f m_LastPosition;
		Quaternion m_lastRotation;
		Vector3f m_lastScale;

		std::vector<Component*> m_components;

		GameObject* parent = nullptr; //If object is a child
		std::vector<GameObject*> m_children;

		glm::mat4 m_fixedNormal;
		glm::mat4 m_transform;

		Sphere m_boundingSphere;
		Mesh* m_mesh;

		friend class Mesh;
		friend class Application;

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
		//Should return the way the Component is displayed in the Inspector.
		virtual const char* getName() = 0;
		//Should use ImGui for an easy way to change values
		virtual void DrawUI() = 0;

	protected:
		GameObject* gameObject;

	};

}

#endif