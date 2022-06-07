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

		void SetTexture(Texture* t) {
			texture = t;
		}

		void Generate() {
			VBO vbo;
			vao.Generate();
			vbo.Generate();
			vbo.SetData(data.vertices, data.size * sizeof(float));
			vao.Bind();
			vao.Link(&vbo, 0, 1);
			vaoGenerated = true;
		}

		void Start() override {
			
		}

		void Update() override {
			if (!vaoGenerated) {
				Generate();
			}
			shader->Use();
			if (texture)
				texture->Bind();
			shader->setMat4("view", Camera::GetMain()->_GetViewMatrix());
			shader->setMat4("projection", Camera::GetMain()->_GetProjectionMatrix());
			shader->setVec3("viewPos", Camera::GetMain()->position.x, Camera::GetMain()->position.y, Camera::GetMain()->position.z);
			shader->setVec3("material.diffuse", shader->diffuse.x, shader->diffuse.y, shader->diffuse.z);
			shader->setVec3("material.specular", shader->specular.x, shader->specular.y, shader->specular.z);
			shader->setFloat("material.shininess", shader->shininess);
			shader->setVec3("lightPos", Scene::activeScene->GetLight()->position.x, Scene::activeScene->GetLight()->position.y, Scene::activeScene->GetLight()->position.z);
			shader->setVec3("light.ambient", Scene::activeScene->GetLight()->ambient.x, Scene::activeScene->GetLight()->ambient.y, Scene::activeScene->GetLight()->ambient.z);
			shader->setVec3("light.diffuse", Scene::activeScene->GetLight()->color.x, Scene::activeScene->GetLight()->color.y,
				Scene::activeScene->GetLight()->color.z);
			shader->setVec3("light.specular", Scene::activeScene->GetLight()->specular.x, Scene::activeScene->GetLight()->specular.y, Scene::activeScene->GetLight()->specular.z);
			glm::mat4 inverse = glm::inverse(gameObject->GetTransformationMatrix());
			glm::mat4 transpose = glm::transpose(inverse);
			gameObject->__SetFixedNormal(transpose);
			shader->setMat4("fixedNormal", gameObject->__GetFixedNormal());
			shader->setMat4("transform", gameObject->GetTransformationMatrix());

			vao.Bind();
			if (gameObject->GetBoundingSphere().isOnFrustum(Camera::GetMain()->GetFrustum(),
				glm::vec3(gameObject->position.x, gameObject->position.y, gameObject->position.z),
				glm::vec3(gameObject->scale.x, gameObject->scale.y, gameObject->scale.z))) {
				glDrawArrays(GL_TRIANGLES, 0, sizeof(float) * data.size);
			}
			if(texture)
				texture->Unbind();
		}

	private:
		MeshData data;
		Shader* shader = nullptr;
		Texture* texture = nullptr;
		VAO vao;
		bool vaoGenerated = false;

	};

}

#endif