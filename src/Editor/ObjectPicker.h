#pragma once
#include "../Math/Vector.h"

#include <memory>
#include <vector>

namespace glGame {

    class Camera;
    class Texture;
    class FrameBuffer;
    class Shader;
    class RenderDataList;

    class ObjectPicker {
    public:
        ObjectPicker(Vector2i resolution);
        ~ObjectPicker();

        int getGameObjectIdFromScreen(const Vector2& normalizedScreenPosition);
        void renderColorPickerTexture(RenderDataList* objRenderData, Camera* camera);

    private:
        Vector2i m_resolution;

        std::shared_ptr<Texture> m_colorPickerTexture;
        std::unique_ptr<FrameBuffer> m_colorPickerFramebuffer;
        std::unique_ptr<Shader> m_colorPickerShader;
    };

}