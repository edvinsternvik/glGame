#include "MeshRenderer.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Resources/AssetManager.h"
#include "../Rendering/Shader.h"
#include <GL/glew.h>

namespace glGame {

	MeshRenderer::MeshRenderer() {
		addPublicVariable(&modelId, "Model");
	}

	void MeshRenderer::init() {
		RendererComponent::init();
	}

	void MeshRenderer::onRender() {
		updateModelMatrix();
	}

	void MeshRenderer::renderComponent(Shader* shader) {
		Model* model = (Model*)AssetManager::Get().getAsset(modelId, AssetType::Model);
		if(model == nullptr) {
			return;
		}
		model->bind();
		shader->setUniformMat4("u_model", &(modelMatrix[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, model->getVerticiesCount());
	}

}