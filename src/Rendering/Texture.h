#pragma once

namespace glGame {

    enum TextureType {
        sRGB, RGB, DEPTH
    };

    class Texture {
    public:
        Texture(float width, float height, TextureType type);
        Texture(const char* filepath);
        Texture(const char* filepath, TextureType textureType);
        Texture(const char* filepath, TextureType textureType, float width, float height);
        Texture(const char* filepath, TextureType textureType, float width, float height, bool generateMipmaps);
        ~Texture();

        void bind();
        void unbind();

        unsigned int getTextureId() const { return m_textureId; }
        unsigned int getWidth() const { return m_imageWidth; }
        unsigned int getHeight() const { return m_imageHeight; }
        unsigned int getChannels() const { return m_channels; }

    public:
        const TextureType textureType;

    private:
        unsigned int m_textureId;
        int m_imageWidth, m_imageHeight, m_channels;
    };

}