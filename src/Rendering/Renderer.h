#pragma once
#include <GL/glew.h>
#include "FrameBuffer.h"
#include "Shader.h"
#include "../Model.h"
#include <memory>

namespace glGame {

	class Scene;

	class Renderer {
	public:
		Renderer();

		void initGLEW();
		void render(std::shared_ptr<Scene>* scene);

		inline unsigned int getFrameTexture() { return m_framebuffer->getTexture(); }

	private:
		void clearScreen();

	private:
		float m_tempTriangleVerticies[6] = {
			-0.5, -0.5, 0.5, -0.5, 0.0, 0.5
		}; 

		float m_renderQuadVerticies[12] = {
			-1.0, -1.0, 1.0, -1.0, 1.0, 1.0,	 1.0, 1.0, -1.0, 1.0, -1.0, -1.0  
		};

		std::unique_ptr<Model> m_tempTriangle;
		std::unique_ptr<Model> m_renderQuad;
		std::unique_ptr<FrameBuffer> m_framebuffer;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<Shader> m_postProcessingShader;

	};
}