#pragma once

namespace glGame {

    class Texture {
    public:
        Texture(const char* filepath);
        ~Texture();

        void bind();
        void unbind();

    private:
        unsigned int m_textureId;
        int m_imageWidth, m_imageHeight, m_channels;
    };

}