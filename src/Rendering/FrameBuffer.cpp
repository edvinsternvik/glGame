#include "FrameBuffer.h"
#include "Texture.h"
#include <GL/glew.h>
#include <iostream>

namespace glGame {

	FrameBuffer::FrameBuffer(const unsigned int& width, const unsigned int& height) {
		glGenFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

		m_texture = std::make_shared<Texture>((float)width, (float)height);
		m_texture->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->getTextureId(), 0);

		m_renderBuffer = genRenderBuffer(width, height);
		glBindRenderbuffer(1, m_renderBuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer not complete" << std::endl;
		}

		unbind();
	}

	FrameBuffer::FrameBuffer(std::shared_ptr<Texture> texture) {
		glGenFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

		GLenum attachment;
		switch(texture->textureType) {
		case TextureType::RGB:
		case TextureType::sRGB:
			attachment = GL_COLOR_ATTACHMENT0;
			break;
		case TextureType::DEPTH:
			attachment = GL_DEPTH_ATTACHMENT;
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		default:
			std::cout << "Error when generating framebuffer - TextureType is invalid" << std::endl;
			break; 
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->getTextureId(), 0);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &m_frameBuffer);
		glDeleteRenderbuffers(1, &m_renderBuffer);
	}

	void FrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		m_texture->bind();
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
	}

	void FrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	unsigned int FrameBuffer::genTexture(const unsigned int& width, const unsigned int& height) {
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;
	}

	unsigned int FrameBuffer::genRenderBuffer(const unsigned int& width, const unsigned int& height) {
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(1, 0);

		return rbo;
	}
}