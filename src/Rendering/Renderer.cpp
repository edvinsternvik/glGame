#include "Renderer.h"

#include <iostream>

namespace glGame {

	Renderer::Renderer() {
		initGLEW();

		m_tempTriangle = std::make_unique<Model>(m_tempTriangleVerticies, 6);
		m_renderQuad = std::make_unique<Model>(m_renderQuadVerticies, 12);
		m_framebuffer = std::make_unique<FrameBuffer>();
		m_shader = std::make_unique<Shader>("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
		m_postProcessingShader = std::make_unique<Shader>("shaders/postProcessing/vertex.glsl", "shaders/postProcessing/fragment.glsl");

		m_shader->useShader();

	}

	void Renderer::render() {
		clearScreen();

		//Render to frame texture
		m_framebuffer->bind();
		clearScreen();

		m_shader->useShader();
		m_tempTriangle->bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//----------------------

		m_framebuffer->unbind();
	}

	void Renderer::initGLEW() {
		if (glewInit() != GLEW_OK) {
			std::cout << "Could not initialize GLEW" << std::endl;
		}
	}

	void Renderer::clearScreen() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

}
