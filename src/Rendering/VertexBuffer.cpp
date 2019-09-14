#include "VertexBuffer.h"
#include <GL/glew.h>

glGame::VertexBuffer::VertexBuffer(float* data, unsigned int arrayLength) {
	glGenBuffers(1, &m_bufferID);
	bindBuffer();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arrayLength, data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void glGame::VertexBuffer::bindBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
}
