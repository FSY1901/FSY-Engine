#include "Mesh.h"
#include "GameObject.h"

namespace FSY {

#pragma region Mesh Vertices
    float Mesh::s_verticesForCube[288] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f
    };
    float Mesh::s_verticesForPlane[48] = {
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
    float Mesh::s_verticesForTriangle[24] = {
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    int Mesh::s_cubeMeshSize = 288;
    int Mesh::s_planeMeshSize = 48;
    int Mesh::s_triangleMeshSize = 24;
#pragma endregion

    Mesh::Mesh() {

    }

    Mesh::Mesh(float* Vertices, int size, Shader* Shader, std::string name) : m_vertices(Vertices), m_size(size), m_shader(Shader), m_name(name) {
        m_objects.reserve(10000);
    }

    Shader* Mesh::GetShader() {
        return this->m_shader;
    }

    Texture* Mesh::GetTexture() {
        return this->m_texture;
    }

    void Mesh::ChangeShader(Shader* Shader) {
        this->m_shader = Shader;
    }

    void Mesh::SetVertices(float* Vertices, int size) {
        this->m_vertices = Vertices;
        this->m_size = size;
    }

    void Mesh::SetTexture(Texture* tex)
    {
        this->m_texture = tex;
        m_hasTexture = true;
    }

    void Mesh::AddGameObject(GameObject* g) {
        g->m_hasMesh = true;
        g->m_mesh = this;
        m_objects.push_back(g);
    }

    void Mesh::RemoveGameObject(GameObject* obj) {
        for (int i = 0; i < m_objects.size(); i++) {
            if (m_objects.at(i) == obj) {
                m_objects.at(i)->m_hasMesh = false;
                m_objects.at(i)->m_mesh = nullptr;
                m_objects.erase(m_objects.begin()+i);
            }
        }
    }

    bool Mesh::HasTexture() {
        return m_hasTexture;
    }

    float* Mesh::GetVertices() {
        return m_vertices;
    }

    int Mesh::GetVertexSize()
    {
        return m_size;
    }

    std::string Mesh::GetName() {
        return m_name;
    }

    void Mesh::SetName(std::string name) {
        m_name = name;
    }

    std::vector<GameObject*> Mesh::_GetGameObjects() {
        return m_objects;
    }

    std::vector<GameObject*>* Mesh::_GetObjectList() {
        return &m_objects;
    }

}