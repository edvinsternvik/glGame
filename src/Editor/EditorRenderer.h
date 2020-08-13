#pragma once
#include "../Math/Vector.h"
#include "../Rendering/Texture.h"

#include <memory>

namespace glGame {

    class FrameBuffer;
	class Shader;

    class EditorRenderer {
    public:
        EditorRenderer(const Vector2i& viewportSize);

        std::shared_ptr<Texture> getFrameTexture();

    private:
        std::shared_ptr<FrameBuffer> m_viewportFrameBuffer;
    };

}