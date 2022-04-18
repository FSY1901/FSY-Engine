#ifndef MESH_RENDERER_H
#define MESH_RENDER_H

#include "../Scene/Mesh.h"
#include "../Scene/GameObject.h"

namespace FSY {

	class FSY_API MeshRenderer : public Component {

	public:

		void SetMeshData(MeshData data, Shader* shader) {
			this->data = data;
			this->shader = shader;
		}

		void Start() override {
			VBO vbo;
			vao.Generate();
			vbo.Generate();
			vbo.SetData(data.vertices, data.size * sizeof(float));
			vao.Bind();
			vao.Link(&vbo, 0, 1);
		}

		void Update() override {

		}

	private:
		MeshData data;
		Shader* shader;
		VAO vao;

	};

}

#endif