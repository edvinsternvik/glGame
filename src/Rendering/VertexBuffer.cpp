#include "VertexBuffer.h"
#include <GL/glew.h>

glGame::VertexBuffer::VertexBuffer() : m_tempData{-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f} {
	bindBuffer();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, m_tempData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
}

void glGame::VertexBuffer::bindBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
}
