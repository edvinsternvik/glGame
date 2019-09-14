#include "Model.h"

namespace glGame {

	Model::Model(float* verticies, unsigned int arrayLen) {
		m_vao = std::make_unique<VertexArray>();
		m_vao->bind();

		m_vbo = std::make_unique<VertexBuffer>(verticies, arrayLen);
		m_vbo->bindBuffer();

		unbind();
	}

	void Model::bind() {
		m_vao->bind();
	}

	void Model::unbind() {
		m_vao->unbind();
	}

}