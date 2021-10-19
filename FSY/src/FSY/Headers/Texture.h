#include "Core.h"

#include "../../vendor/OpenGL/include/glad.h"
#include <vector>

#ifndef TEXTURE_H
#define TEXTURE_H

namespace FSY {

	class FSY_API Texture {

	public:
		Texture();
		Texture(const char* filename);
		void Bind();
		void LoadTexture();

	private:
		const char* m_filename;
		GLuint m_ttexture;

	};

}

#endif 