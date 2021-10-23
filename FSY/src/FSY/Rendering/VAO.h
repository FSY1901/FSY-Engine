#ifndef VAO_H
#define VAO_H

#include "../Core/Core.h"
#include "../../vendor/OpenGL/include/glad.h"
#include "VBO.h"

namespace FSY {

	class VAO {

	public:
		VAO();
		void Generate();
		void Link(VBO* VBO, GLuint layout_pos, GLuint layout_tex);
		void Bind();
		void Unbind();
		void Delete();

	private:
		unsigned int m_ID;

	};

}

#endif