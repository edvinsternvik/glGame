#include "FrameBuffer.h"
#include <GL/glew.h>
#include <iostream>

glGame::FrameBuffer::FrameBuffer() {
	genFramebuffer();
}

glGame::FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &m_frameBuffer);
}

void glGame::FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glBindTexture(GL_TEXTURE_2D, m_frameTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
}

void glGame::FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void glGame::FrameBuffer::genFramebuffer() {
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	m_frameTexture = genTexture(1280, 720);
	glBindTexture(GL_TEXTURE_2D, m_frameTexture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameTexture, 0);

	m_renderBuffer = genRenderBuffer(1280, 720);
	glBindRenderbuffer(1, m_renderBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete" << std::endl;
	}

	unbind();
}

unsigned int glGame::FrameBuffer::genTexture(int width, int height) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

unsigned int glGame::FrameBuffer::genRenderBuffer(int width, int height) {
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(1, 0);

	return rbo;
}