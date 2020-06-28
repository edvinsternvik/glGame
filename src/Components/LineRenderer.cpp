#include "LineRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../GameObject.h"
#include "Transform.h"
#include "../Rendering/Shader.h"
#include <GL/glew.h>

namespace glGame {

	LineRenderer::LineRenderer() {
        addPublicVariable(&lineLength, PublicVariableType::Vec3, "LineLength");
        addPublicVariable(&lineWidth, PublicVariableType::Float, "LineWidth");
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

	void LineRenderer::renderComponent(Shader* shader) {
        shader->setUniformMat4("u_model", &(modelMatrix[0][0]));

		m_vao->bind();
        m_vbo->updateData(&lineLength.x);
		glLineWidth(lineWidth);
		glDrawArrays(GL_LINES, 0, 2);
		m_vao->unbind();
	}

}