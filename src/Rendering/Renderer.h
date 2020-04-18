#pragma once
#include <GL/glew.h>
#include "FrameBuffer.h"
#include "Shader.h"
#include "../Resources/Model.h"
#include <memory>
#include <vector>

namespace glGame {

	class Scene;
	class GameObject;

	class Renderer {
	public:
		Renderer();

		void initGLEW();

		void beginRender();
		void render(Scene* scene);
		void renderGizmos(const std::vector<GameObject*>& gizmoObjects);
		void endRender();

		inline unsigned int getEditorFrameTexture() { return m_editorFramebuffer->getTexture(); }

	private:
		void clearScreen();

	private:

		float m_renderQuadVerticies[12] = {
			-1.0, -1.0, 1.0, -1.0, 1.0, 1.0,	 1.0, 1.0, -1.0, 1.0, -1.0, -1.0  
		};

		// std::unique_ptr<Model> m_renderQuad;
		std::unique_ptr<FrameBuffer> m_editorFramebuffer;
		std::unique_ptr<Shader> m_shader;
		// std::unique_ptr<Shader> m_postProcessingShader;

	};
}