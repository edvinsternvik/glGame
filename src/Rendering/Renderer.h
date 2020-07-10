#pragma once
#include <GL/glew.h>
#include "FrameBuffer.h"
#include "Shader.h"
#include "../Resources/Model.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace glGame {

	class Scene;
	class GameObject;
	class Camera;
	class Material;

	struct ObjectRenderData {
	public:
		ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Material* material, const glm::mat4& modelMatrix)
			: vao(vao), verticies(verticies), material(material), modelMatrix(modelMatrix) {}

		VertexArray* vao;
		unsigned int verticies;
		Material* material;
		glm::mat4 modelMatrix;
	};

	class Renderer {
	public:
		void init();

		void submit(Model* model, const glm::mat4& modelMatrix);
		void submit(VertexArray* vertexArray, const unsigned int& verticies, const glm::mat4& modelMatrix);

		void beginRender(Camera* camera);
		void render(Scene* scene);
		void renderGizmos(const std::vector<GameObject*>& gizmoObjects);
		void endRender();

		inline unsigned int getEditorFrameTexture() { return m_editorFramebuffer->getTexture(); }

	private:
		void initGLEW();
		void clearScreen();

	private:

		float m_renderQuadVerticies[12] = {
			-1.0, -1.0, 1.0, -1.0, 1.0, 1.0,	 1.0, 1.0, -1.0, 1.0, -1.0, -1.0  
		};

		// std::unique_ptr<Model> m_renderQuad;
		std::unique_ptr<FrameBuffer> m_editorFramebuffer;
		std::unique_ptr<Shader> m_shader;
		std::vector<ObjectRenderData> m_renderData;
		// std::unique_ptr<Shader> m_postProcessingShader;

	};
}