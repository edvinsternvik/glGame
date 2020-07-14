#include "Model.h"
#include "../Resources/ModelLoader.h"

namespace glGame {

	Model::Model(const char* fileName) {
		m_vao = std::make_unique<VertexArray>();
		m_vao->bind();

		std::vector<ModelData> vertexData;
		std::vector<unsigned int> indexData;

		ModelLoader::loadModel(fileName, &vertexData, &indexData);

		m_verticies = vertexData.size();
		m_vbo = std::make_unique<VertexBuffer>((float*)&(vertexData[0].vertex.x), m_verticies * 8);

		m_vbo->addAttribute(3, 8);
		m_vbo->addAttribute(2, 8);
		m_vbo->addAttribute(3, 8);
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