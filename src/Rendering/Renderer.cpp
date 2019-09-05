#include "Renderer.h"

namespace glGame {

	Renderer::Renderer() {
	}

	void Renderer::render() {
		clearScreen();
	}

	void Renderer::clearScreen() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

}
