#include "MeshRenderer.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Resources/AssetManager.h"
#include "../Application.h"
#include <GL/glew.h>

namespace glGame {

	MeshRenderer::MeshRenderer() {
		addPublicVariable(&model, "Model");
		addPublicVariable(&material, "Material");
	}

	void MeshRenderer::init() {
		RendererComponent::init();
	}

	void MeshRenderer::onRender() {
		if(!model.expired() && !material.expired()) {
			updateModelMatrix();
			Application::Get().renderer.submit(model.get(), modelMatrix, material.get(), getGameObject()->id);
		}
	}

}