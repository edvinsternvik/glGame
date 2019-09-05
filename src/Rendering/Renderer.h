#pragma once
#include <GL/glew.h>

namespace glGame {

	class Renderer {
	public:
		Renderer();

		void render();

	private:
		void clearScreen();
	};
}