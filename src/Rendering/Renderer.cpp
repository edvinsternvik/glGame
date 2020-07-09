#include "Renderer.h"
#include "../Resources/Scene.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"

#include <iostream>

namespace glGame {

	void Renderer::init() {
		initGLEW();

		#ifdef GL_GAME_EDITOR
		m_editorFramebuffer = std::make_unique<FrameBuffer>();
		#endif

		m_shader = std::make_unique<Shader>("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

		m_shader->useShader();
	}

	void Renderer::submit(Model* model, const glm::mat4& modelMatrix) {
		if(model == nullptr) return;
		model->bind();
		m_shader->setUniformMat4("u_model", &(modelMatrix[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, model->getVerticiesCount());
	}

	void Renderer::submit(VertexArray* vertexArray, const unsigned int& verticies, const glm::mat4& modelMatrix) {
		if(vertexArray == nullptr) return;

		vertexArray->bind();
		m_shader->setUniformMat4("u_model", &(modelMatrix[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, verticies);
	}

	void Renderer::beginRender(Camera* camera) {
		clearScreen();

		#ifdef GL_GAME_EDITOR
		//Render to frame texture
		m_editorFramebuffer->bind();
		#endif

		m_shader->useShader();
		if(camera != nullptr) {
			m_shader->setUniformMat4("u_projection", &(camera->getProjectionMatrix()[0][0]));
			m_shader->setUniformMat4("u_view", &(camera->getViewMatrix()[0][0]));
		}

		clearScreen();
	}

	void Renderer::render(Scene* scene) {
		//Render scene
		scene->onRender();
	}

	void Renderer::renderGizmos(const std::vector<GameObject*>& gizmoObjects) {
		glClear(GL_DEPTH_BUFFER_BIT);
		for(GameObject* go : gizmoObjects) {
			go->onRender();
		}
	}

	void Renderer::endRender() {
		#ifdef GL_GAME_EDITOR
		m_editorFramebuffer->unbind();
		#endif
	}

	void Renderer::initGLEW() {
		if (glewInit() != GLEW_OK) {
			std::cout << "Could not initialize GLEW" << std::endl;
			return;
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::clearScreen() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}
