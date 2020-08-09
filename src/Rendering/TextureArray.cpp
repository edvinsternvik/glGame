#include "TextureArray.h"
#include <GL/glew.h>

namespace glGame {

    TextureArray::TextureArray(unsigned int textures, unsigned int width, unsigned int height) : textures(textures), width(width), height(height) {
        glGenTextures(1, &m_textureArrayId);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureArrayId);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32, width, height, textures, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    }

    TextureArray::~TextureArray() {
        glDeleteTextures(1, &m_textureArrayId);
        m_textureArrayId = -1;
    }

    void TextureArray::bind() {
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureArrayId);
    }

    void TextureArray::unbind() {
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

}