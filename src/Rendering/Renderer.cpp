#include "Renderer.h"
#include "../Scene.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"

#include <iostream>

namespace glGame {

	Renderer::Renderer() {
		initGLEW();

		// m_renderQuad = std::make_unique<Model>(m_renderQuadVerticies, 12);
		m_framebuffer = std::make_unique<FrameBuffer>();
		m_shader = std::make_unique<Shader>("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
		// m_postProcessingShader = std::make_unique<Shader>("shaders/postProcessing/vertex.glsl", "shaders/postProcessing/fragment.glsl");

		m_shader->useShader();

	}

	void Renderer::render(std::shared_ptr<Scene>* scene) {
		clearScreen();

		//Render to frame texture
		m_framebuffer->bind();
		clearScreen();

		//Render scene
		(*scene)->onRender();
		m_shader->useShader();
		Camera* camera = (*scene)->activeCamera;
		m_shader->setUniformMat4("u_projection", &(camera->getProjectionMatrix()[0][0]));
		m_shader->setUniformMat4("u_view", &(camera->getViewMatrix()[0][0]));

		//Render GameObjects
		int gameObjectCount = (*scene)->getGameObjectCount();
		for(int i = 0; i < gameObjectCount; ++i) {
			MeshRenderer* meshRenderer = (*scene)->getGameObject(i)->meshRenderer;
			Transform* transform = (*scene)->getGameObject(i)->transform;
			if(meshRenderer) {
				
				//Render object
				meshRenderer->model->bind();
				m_shader->setUniformMat4("u_model", &(meshRenderer->modelMatrix[0][0]));
				glDrawArrays(GL_TRIANGLES, 0, 3);

			}
		}
		//----------------------

		m_framebuffer->unbind();
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
