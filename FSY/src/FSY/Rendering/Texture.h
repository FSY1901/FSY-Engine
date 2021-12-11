#include "../Core/Core.h"

#include "../../vendor/OpenGL/include/glad.h"
#include <vector>

#ifndef TEXTURE_H
#define TEXTURE_H

namespace FSY {

	class FSY_API Texture {

	public:
		Texture();
		Texture(const char* filename, bool flipped=true);
		void Bind();
		void Unbind();
		GLuint GetTexture();

	private:
		const char* m_filename;
		GLuint m_texture;

	};

}

#endif 