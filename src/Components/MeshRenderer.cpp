#include "MeshRenderer.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Resources/AssetManager.h"
#include "../Application.h"
#include <GL/glew.h>

namespace glGame {

	MeshRenderer::MeshRenderer() {
		addPublicVariable(&model, "Model");
	}

	void MeshRenderer::init() {
		RendererComponent::init();
	}

	void MeshRenderer::onRender() {
		if(!model.expired()) {
			updateModelMatrix();
			Application::Get().renderer.submit(model.get(), modelMatrix);
		}
	}

}