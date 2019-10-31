#include "MeshRenderer.h"

namespace glGame {

	MeshRenderer::MeshRenderer() {
		model = std::make_unique<Model>(m_tempTriangleVerticies, 6);
	}

	void MeshRenderer::update() {

	}	

}