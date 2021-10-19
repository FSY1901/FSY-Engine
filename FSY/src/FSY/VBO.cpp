#include "VBO.h"

namespace FSY {

	VBO::VBO() {
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void VBO::Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void VBO::SetData(float* vertices, GLsizeiptr size) {
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	void VBO::Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VBO::Delete() {
		glDeleteBuffers(1, &m_ID);
	}

}