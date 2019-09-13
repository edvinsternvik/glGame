#include "Renderer.h"

#include <iostream>

namespace glGame {

	Renderer::Renderer() {
		initGLEW();

		tempvb = std::make_unique<VertexBuffer>();
		m_framebuffer = std::make_unique<FrameBuffer>();
	}

	void Renderer::render() {
		clearScreen();

		glDrawArrays(GL_TRIANGLES, 0, 6);
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
