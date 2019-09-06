#pragma once

namespace glGame {

	class VertexBuffer {
	public:
		VertexBuffer();

		void bindBuffer();

	private:
		unsigned int m_bufferID;

		float m_tempData[6];
	};

}