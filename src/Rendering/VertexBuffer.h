#pragma once

namespace glGame {

	class VertexBuffer {
	public:
		VertexBuffer(float* data, unsigned int arrayLength);

		void bindBuffer();
		void unbindBuffer();
		void updateData(float* data);
		void setData(float* data, int size);
		void addAttribute(int count, int stride);

	private:
		unsigned int m_bufferID, m_attributes, m_attributesEnd, m_dataSize;
	};

}