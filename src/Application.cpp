#include "Application.h"
#include <iostream>

namespace glGame {

	Application::Application() {
		std::string title = "glGame";
		int width = 1280, height = 720;
		m_viewportAspectRatio = width / (float)height;
		m_window = std::make_unique<Window>(title, width, height);
		m_window->setEventFunction(std::bind(&Application::onEvent, this, std::placeholders::_1));

		m_renderer = std::make_unique<Renderer>(); // Initializes glew, has to be called before any opengl code

		m_sceneManager = std::make_unique<SceneManager>();
		m_sceneManager->initializeScene();

		#ifdef GL_GAME_EDITOR
		m_editor = std::make_unique<Editor>(std::bind(&Application::onEvent, this, std::placeholders::_1), m_window.get(), m_renderer->getEditorFrameTexture(), m_viewportAspectRatio);
		#endif
	}

	void Application::run() {
		#ifdef GL_GAME_EDITOR
		m_editor->setActiveScene(m_sceneManager->getActiveScene());
		#endif

		while(m_running) {
			m_sceneManager->initScene();

			float deltatime = m_time.getDeltatime();
			if(m_focused) {
				#ifndef GL_GAME_EDITOR
				m_sceneManager->updateScene(deltatime);
				#else
				m_editor->update(deltatime);
				#endif
			}

			m_renderer->render(m_sceneManager->getActiveScene());
			
			#ifdef GL_GAME_EDITOR
			m_editor->render();
			#endif

			Input::update();
			m_window->pollEvents();
			m_window->swapBuffers();
		}
	}

	void Application::onEvent(Event& e) {
		if(e.isInCategory(EventCategory::Input)) {
			Input::handleEvent(&e);
		}

		else if(e.isInCategory(EventCategory::Application)) {
			if(e.getEventType() == EventType::WindowClose) {
				m_running = false;
			}
		}

		else if(e.isInCategory(EventCategory::Editor)) {
			if(e.getEventType() == EventType::SaveScene) {
				std::cout << "Scene saved" << std::endl;
				m_sceneManager->saveActiveScene();
			}
			else if(e.getEventType() == EventType::GameFocused) {
				m_focused = true;
			}
			else if(e.getEventType() == EventType::GameUnfocused) {
				m_focused = false;
			}
		}
	}
}