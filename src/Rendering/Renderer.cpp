#include "Renderer.h"
#include "../Resources/Scene.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"

#include <iostream>

namespace glGame {

	Renderer::Renderer() {
		initGLEW();

		#ifdef GL_GAME_EDITOR
		m_editorFramebuffer = std::make_unique<FrameBuffer>();
		#endif

		m_shader = std::make_unique<Shader>("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

		m_shader->useShader();

	}

	void Renderer::render(Scene* scene) {
		clearScreen();

		#ifdef GL_GAME_EDITOR
		//Render to frame texture
		m_editorFramebuffer->bind();
		#endif

		clearScreen();

		//Render scene
		scene->onRender();
		m_shader->useShader();
		Camera* camera = scene->activeCamera;
		m_shader->setUniformMat4("u_projection", &(camera->getProjectionMatrix()[0][0]));
		m_shader->setUniformMat4("u_view", &(camera->getViewMatrix()[0][0]));

		//Render GameObjects
		int gameObjectCount = scene->getGameObjectCount();
		for(int i = 0; i < gameObjectCount; ++i) {
			MeshRenderer* meshRenderer = scene->getGameObject(i)->meshRenderer;
			Transform* transform = scene->getGameObject(i)->transform;
			if(meshRenderer) {
				
				//Render object
				meshRenderer->model->bind();
				m_shader->setUniformMat4("u_model", &(meshRenderer->modelMatrix[0][0]));
				glDrawArrays(GL_TRIANGLES, 0, meshRenderer->model->getVerticiesCount());

			}
		}
		//----------------------

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
