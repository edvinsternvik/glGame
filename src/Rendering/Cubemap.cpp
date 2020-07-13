#include "Cubemap.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include <stb_image/stb_image.h>
#include <GL/glew.h>

#include <iostream>

namespace glGame {

    Cubemap::Cubemap(const std::array<std::string, 6>& texturePaths) {
        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

        for(int i = 0; i < 6; ++i) {
            stbi_set_flip_vertically_on_load(false);

            unsigned char* data = stbi_load(texturePaths[i].c_str(), &m_imageWidth, &m_imageHeight, &m_channels, 3);
            if(data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_imageWidth, m_imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else {
                std::cout << "Could not load image " << texturePaths[i] << std::endl;
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void Cubemap::bind() {
        Cubemap::getVertexArray()->bind();
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
    }
    void Cubemap::unbind() {
        Cubemap::getVertexArray()->unbind();
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    VertexArray* Cubemap::getVertexArray() {
        if(!s_cubemapVAO.get()) {
            s_cubemapVAO = std::make_unique<VertexArray>();
            s_cubemapVAO->bind();
            VertexBuffer vbo(s_cubemapVertices, 108);
            vbo.addAttribute(3, 3);
            vbo.bindBuffer();
            s_cubemapVAO->unbind();
        }
        return s_cubemapVAO.get();
    }

    std::unique_ptr<VertexArray> Cubemap::s_cubemapVAO;
    
    float Cubemap::s_cubemapVertices[] = {       
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

}