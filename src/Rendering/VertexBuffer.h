#pragma once

namespace glGame {

	class VertexBuffer {
	public:
		VertexBuffer(float* data, unsigned int arrayLength);

		void bindBuffer();

	private:
		unsigned int m_bufferID;
	};

}