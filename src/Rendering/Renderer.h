#pragma once
#include <GL/glew.h>
#include "VertexBuffer.h"

#include <memory>

namespace glGame {

	class Renderer {
	public:
		Renderer();

		void initGLEW();
		void render();

	private:
		void clearScreen();

	private:
		std::unique_ptr<VertexBuffer> tempvb;
		
	};
}