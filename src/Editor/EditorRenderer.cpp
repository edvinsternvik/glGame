#include "EditorRenderer.h"
#include "../Application.h"
#include "../Rendering/FrameBuffer.h"
#include "../Rendering/Texture.h"
#include "../Rendering/Shader.h"

namespace glGame {

    EditorRenderer::EditorRenderer(const Vector2i& viewportSize) : m_viewportFrameBuffer(std::make_shared<FrameBuffer>(viewportSize.x, viewportSize.y)) {
        Application::Get().renderer.setDefaultRenderTarget(m_viewportFrameBuffer);
    }

    std::shared_ptr<Texture> EditorRenderer::getFrameTexture() {
        return m_viewportFrameBuffer->getTexture();
    }



}