#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../vendor/OpenGL/include/glad.h"

namespace FSY {

	class Framebuffer {

	public:

		int width, height;

		Framebuffer();

		void Generate(int x, int y);
		void Bind();
		void Unbind();
		void Draw();
		void Delete();
		void Resize(int x, int y);
		unsigned int GetVAO();
		unsigned int GetTexture();

	private:
		unsigned int m_ID = 0;
		unsigned int m_texture = 0;
		unsigned int m_RBO = 0;
		unsigned int m_VAO = 0;

	};

}

#endif