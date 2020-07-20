#pragma once

namespace glGame {

    enum TextureType {
        sRGB, RGB
    };

    class Texture {
    public:
        Texture(const char* filepath, TextureType textureType);
        ~Texture();

        void bind();
        void unbind();

    private:
        unsigned int m_textureId;
        int m_imageWidth, m_imageHeight, m_channels;
    };

}