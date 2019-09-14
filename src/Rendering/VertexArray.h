#pragma once

namespace glGame {

	class VertexArray {
	public:
		VertexArray();

		void bind();
		void unbind();

	private:
		unsigned int m_vertexArrayID;
	};

}