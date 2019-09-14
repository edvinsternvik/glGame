#include "VertexArray.h"
#include <GL/glew.h>

glGame::VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_vertexArrayID);
}

void glGame::VertexArray::bind() {
	glBindVertexArray(m_vertexArrayID);
}

void glGame::VertexArray::unbind() {
	glBindVertexArray(0);
}
