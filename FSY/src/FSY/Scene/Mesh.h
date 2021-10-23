#ifndef MESH_H
#define MESH_H

#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"

#include <vector>

namespace FSY {

#pragma region Render Modes
	FSY_API enum RenderModes {
		RENDER_FRONT = 0,
		RENDER_BOTH
	};
#pragma endregion

	extern class GameObject;

	class FSY_API Mesh {

	public:
		Mesh();
		Mesh(float* Vertices, int size, Shader* Shader);
		Shader* GetShader();
		Texture* GetTexture();
		//Change the shader with which the object is rendered
		void ChangeShader(Shader* Shader);
		void SetTexture(Texture* tex);
		void SetVertices(float* Vertices, int size);
		void AddGameObject(GameObject* g);
		/// <summary>
		/// Returns true if the Mesh has a Texture.
		/// </summary>
		bool HasTexture();
		/// <summary>
		/// Changes if only the front of a face is rendered(in the normal direction) or both sides.
		/// </summary>
		float* GetVertices();
		int GetVertexSize();

		int renderMode = RenderModes::RENDER_FRONT;
		bool isTransparent = false;

		std::vector<GameObject*> _GetGameObjects();

#pragma region MESHES
		static float s_verticesForCube[288];
		static float s_verticesForPlane[48];
		static int s_cubeMeshSize;
		static int s_planeMeshSize;
#pragma endregion

	private:
		int m_size;
		float* m_vertices;
		Shader* m_shader = nullptr;
		Texture* m_texture = nullptr;
		bool m_hasTexture = false;

		std::vector<GameObject*> m_objects = {};

	};

}

#endif