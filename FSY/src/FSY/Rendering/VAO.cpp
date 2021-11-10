#include "VAO.h"

namespace FSY {

    VAO::VAO() {

    }

    void VAO::Generate() {
        glGenVertexArrays(1, &m_ID);
    }

    void VAO::Link(VBO* VBO, GLuint layout_pos, GLuint layout_tex) {
        VBO->Bind();
        
        //position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //light attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        //texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        VBO->Unbind();
    }

    void VAO::Bind() {
        glBindVertexArray(m_ID);
    }

    void VAO::Unbind() {
        glBindVertexArray(0);
    }

    void VAO::Delete() {
        glDeleteVertexArrays(1, &m_ID);
    }

}