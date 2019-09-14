#pragma once
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"

#include <memory>

namespace glGame {

	class Model {
	public:
		Model(float* verticies, unsigned int arrayLen);

		void bind();
		void unbind();

	private:
		std::unique_ptr<VertexArray> m_vao;
		std::unique_ptr<VertexBuffer> m_vbo;
	};

}