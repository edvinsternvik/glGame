#pragma once
#include <GL/glew.h>
#include "VertexBuffer.h"
#include "FrameBuffer.h"
#include "Shader.h"

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
		std::unique_ptr<FrameBuffer> m_framebuffer;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<Shader> m_postProcessingShader;
	};
}