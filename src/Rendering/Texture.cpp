#include "Texture.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <iostream>

namespace glGame {

    Texture::Texture(const char* filepath, TextureType textureType = TextureType::sRGB) {
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(filepath, &m_imageWidth, &m_imageHeight, &m_channels, 0);
        if(!data) {
            std::cout << "Could not load texture: " << filepath << std::endl;
            return;
        }

        glGenTextures(1, &m_textureId);
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLint internalFormat, format;
        if(m_channels == 3) {
            format = internalFormat = GL_RGB;
            if(textureType == TextureType::sRGB) internalFormat = GL_SRGB;
        }
        else if(m_channels == 4) {
            format = internalFormat = GL_RGBA;
            if(textureType == TextureType::sRGB) internalFormat = GL_SRGB_ALPHA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_imageWidth, m_imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_textureId);
    }

    void Texture::bind() {
        glBindTexture(GL_TEXTURE_2D, m_textureId);
    }

    void Texture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}