#ifndef VBO_H
#define VBO_H

#include "../Core/Core.h"
#include "../../vendor/OpenGL/include/glad.h"

namespace FSY {

	class FSY_API VBO {

	public:
		VBO();
		void SetData(float* vertices, GLsizeiptr size);
		unsigned int getID();

		void Bind();
		void Unbind();
		void Delete();

	private:
		unsigned int m_ID;

	};

}

#endif 