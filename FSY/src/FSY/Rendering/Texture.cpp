#include "Texture.h"
#include "../../vendor/stb_image/stb_image.h"
#include <iostream>

namespace FSY {

    Texture::Texture() {

    }

    Texture::Texture(const char* filename) {
        this->m_filename = filename;
        glGenTextures(1, &m_ttexture);
        glBindTexture(GL_TEXTURE_2D, m_ttexture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // load and generate the texture
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        //for pngs
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, STBI_rgb_alpha);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void Texture::Bind() {
        //glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, m_ttexture);
    }

    void Texture::LoadTexture() {

    }

}