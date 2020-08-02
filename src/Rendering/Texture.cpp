#include "Texture.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <iostream>

namespace glGame {

    Texture::Texture(float width, float height) : Texture(nullptr, TextureType::RGB, width, height) {

    }

    Texture::Texture(const char* filepath) : Texture(filepath, TextureType::sRGB) {

    }

    Texture::Texture(const char* filepath, TextureType textureType) : Texture(filepath, textureType, -1.0, -1.0) {

    }

    Texture::Texture(const char* filepath, TextureType textureType, float width, float height) : Texture(filepath, textureType, width, height, true) {

    }


    Texture::Texture(const char* filepath, TextureType textureType, float width, float height, bool generateMipmaps)
    : textureType(textureType), m_imageWidth(width), m_imageHeight(height) {
        unsigned char* data = NULL;

        glGenTextures(1, &m_textureId);
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if(filepath) {
            stbi_set_flip_vertically_on_load(true);

            data = stbi_load(filepath, &m_imageWidth, &m_imageHeight, &m_channels, 0);
            if(!data) {
                std::cout << "Could not load texture: " << filepath << std::endl;
                return;
            }


        }

        GLint internalFormat, format, type = GL_UNSIGNED_BYTE;
        switch(textureType) {
        case TextureType::RGB:
        case TextureType::sRGB:
            if(m_channels == 4) {
                format = internalFormat = GL_RGBA;
                if(textureType == TextureType::sRGB) internalFormat = GL_SRGB_ALPHA;
            }
            else {
                format = internalFormat = GL_RGB;
                if(textureType == TextureType::sRGB) internalFormat = GL_SRGB;
            }
            break;
        case TextureType::DEPTH:
            format = internalFormat = GL_DEPTH_COMPONENT;
            type = GL_FLOAT;
            break;
        default:
            std::cout << "Error - Invalid texture type" << std::endl;
            return;
        }

        if(m_imageWidth <= 0 || m_imageHeight <= 0) {
            std::cout << "Error - Texture width and height has to be larger than zero" << std::endl;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_imageWidth, m_imageHeight, 0, format, type, data);
        if(generateMipmaps) glGenerateMipmap(GL_TEXTURE_2D);

        if(filepath) stbi_image_free(data);
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