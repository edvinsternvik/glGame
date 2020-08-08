#pragma once

namespace glGame {

    class TextureArray {
    public:
        TextureArray(unsigned int textures, unsigned int width, unsigned int height);
        ~TextureArray();

        unsigned int getId() const { return m_textureArrayId; }
        void bind();
        void unbind();

    public:
        const unsigned int textures, width, height;

    private:
        unsigned int m_textureArrayId;
    };

}