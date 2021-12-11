#include "Framebuffer.h"
#include <iostream>

namespace FSY {

	Framebuffer::Framebuffer() {

	}

	void Framebuffer::Generate(int x, int y) {

		if (m_ID != 0) {
			Delete();
		} 
		else {
			float rectangleVertices[] = //covers the whole screen
			{
				// Coords    // texCoords
				 1.0f, -1.0f,  1.0f, 0.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				-1.0f,  1.0f,  0.0f, 1.0f,

				 1.0f,  1.0f,  1.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				-1.0f,  1.0f,  0.0f, 1.0f
			};

			// Prepare framebuffer rectangle VBO and VAO
			unsigned int rectVBO;
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &rectVBO);
			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		}


		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
	}

	void Framebuffer::Bind() {
		//glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}

	void Framebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Draw() {
		glBindVertexArray(m_VAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindBuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Delete(){
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_texture);
		glDeleteTextures(1, &m_RBO);
	}

	void Framebuffer::Resize(int x, int y) {
		width = x;
		height = y;
		Generate(x, y);
	}

	unsigned int Framebuffer::GetTexture() { return m_texture; }

}