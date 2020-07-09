#include "LineRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../GameObject.h"
#include "Transform.h"
#include "../Rendering/Shader.h"
#include <GL/glew.h>

namespace glGame {

	LineRenderer::LineRenderer() {
        addPublicVariable(&lineLength, "LineLength");
        addPublicVariable(&lineWidth, "LineWidth");
	}

	void LineRenderer::init() {
		RendererComponent::init();
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();
        m_vbo = std::make_unique<VertexBuffer>(&lineLength.x, 3);
        m_vbo->addAttribute(3, sizeof(float) * 3);
        m_vbo->bindBuffer();
        m_vao->unbind();
	}

	void LineRenderer::onRender() {
        updateModelMatrix();
	}

}