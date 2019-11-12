#include "VertexBuffer.h"
#include <GL/glew.h>

glGame::VertexBuffer::VertexBuffer(float* data, unsigned int arrayLength) : m_attributes(0), m_attributesEnd(0) {
	glGenBuffers(1, &m_bufferID);
	bindBuffer();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arrayLength, data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void glGame::VertexBuffer::bindBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
}

void glGame::VertexBuffer::addAttribute(int count, int stride) {
	bindBuffer();

	glEnableVertexAttribArray(m_attributes);
	glVertexAttribPointer(m_attributes++, count, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (const void*)(m_attributesEnd));
	m_attributesEnd += sizeof(float) * count;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
