#include "MeshRenderer.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Rendering/Shader.h"
#include <GL/glew.h>

namespace glGame {

	MeshRenderer::MeshRenderer() {
		addPublicVariable(&m_modelFilePath, PublicVariableType::String, "Modelfile");
	}

	void MeshRenderer::init() {
		model = std::make_unique<Model>(m_modelFilePath.c_str());
	}

	void MeshRenderer::onRender() {
		updateModelMatrix();
	}

	void MeshRenderer::renderComponent(Shader* shader) {
		model->bind();
		shader->setUniformMat4("u_model", &(modelMatrix[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, model->getVerticiesCount());
	}

}