#pragma once
#include <memory>
#include <vector>
#include <array>

namespace glGame {

    class Light;
    class Shader;
    class TextureArray;
    class UniformBuffer;

    class LightManager {
    public:
        LightManager();
        ~LightManager();

        void updateLight(std::shared_ptr<Light> light);
		void deleteLight(const unsigned int& lightid);

    public:
        static const unsigned int MAX_SHADOWMAPS = 8;

        std::unique_ptr<Shader> shadowmapShader;
		std::shared_ptr<TextureArray> m_shadowmapTextureArray;
		std::vector<std::shared_ptr<Light>> m_lights;

    private:
        unsigned int m_lightCountOffset;
		unsigned int m_shadowmapCount = 0;
		std::array<bool, MAX_SHADOWMAPS> m_shadowmaps;

        std::unique_ptr<UniformBuffer> m_lightsUniformBuffer;
    };

}
