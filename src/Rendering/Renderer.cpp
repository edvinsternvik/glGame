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

	void Renderer::beginRender() {
		clearScreen();

		#ifdef GL_GAME_EDITOR
		//Render to frame texture
		m_editorFramebuffer->bind();
		#endif

		clearScreen();
	}

	void Renderer::render(Scene* scene) {
		//Render scene
		scene->onRender();
		m_shader->useShader();
		Camera* camera = scene->activeCamera;
		if(camera != nullptr) {
			m_shader->setUniformMat4("u_projection", &(camera->getProjectionMatrix()[0][0]));
			m_shader->setUniformMat4("u_view", &(camera->getViewMatrix()[0][0]));
		}

		//Render GameObjects
		int gameObjectCount = scene->getGameObjectCount();
		for(int i = 0; i < gameObjectCount; ++i) {
			for(RendererComponent* rendererComponent : scene->getGameObject(i)->rendererComponents) {
				if(rendererComponent) {
					rendererComponent->renderComponent(m_shader.get());
				}
			}
		}
		//----------------------
	}

	void Renderer::renderGizmos(const std::vector<GameObject*>& gizmoObjects) {
		glClear(GL_DEPTH_BUFFER_BIT);
		for(GameObject* go : gizmoObjects) {
			for(RendererComponent* rc : go->rendererComponents) {
				rc->renderComponent(m_shader.get());
			}
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
