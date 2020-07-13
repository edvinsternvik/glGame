#pragma once
#include <memory>
#include <string>
#include <array>

namespace glGame {

    class VertexArray;

    class Cubemap {
    public:
        Cubemap(const std::array<std::string, 6>& texturePaths);

        void bind();
        void unbind();

        static VertexArray* getVertexArray();
    private:

    private:
        unsigned int m_textureId;
        int m_imageWidth, m_imageHeight, m_channels;

        static float s_cubemapVertices[];
        static std::unique_ptr<VertexArray> s_cubemapVAO;
    };

}