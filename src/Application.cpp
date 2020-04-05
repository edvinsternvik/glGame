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

		setUpEditor();
	}

	void Application::run() {
		#ifdef GL_GAME_EDITOR
		m_sceneManager->getActiveScene()->activeCamera = (Camera*)m_sceneManager->getActiveScene()->editorCameraObject->getComponent(1);
		#endif

		while(m_running) {
			m_sceneManager->initScene();

			#ifndef GL_GAME_EDITOR
			m_sceneManager->updateScene();
			#else
			m_sceneManager->getActiveScene()->editorCameraObject->onUpdate();
			#endif

			m_renderer->render(m_sceneManager->getActiveScene());
			
			#ifdef GL_GAME_EDITOR
			m_editorGui->OnGuiRender();
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

		if(e.isInCategory(EventCategory::Application)) {
			if(e.getEventType() == EventType::WindowClose) {
				m_running = false;
			}
		}

		if(e.isInCategory(EventCategory::Editor)) {
			if(e.getEventType() == EventType::SaveScene) {
				std::cout << "Scene saved" << std::endl;
				m_sceneManager->saveActiveScene();
			}
		}
	}

	#ifdef GL_GAME_EDITOR
	void Application::setUpEditor() {
		m_editorGui = std::make_unique<Gui>(m_window->getWindow());
		m_editorGui->m_windows.push_back(std::make_unique<ViewportWindow>(m_renderer->getEditorFrameTexture(), m_viewportAspectRatio));
		m_editorGui->m_windows.push_back(std::make_unique<SceneWindow>(m_sceneManager->getActiveScene()));
		m_editorGui->m_windows.push_back(std::make_unique<PropertiesWindow>(m_sceneManager->getActiveScene()));
		m_editorGui->setEventFunction(std::bind(&Application::onEvent, this, std::placeholders::_1));
	}
	#else
	void Application::setUpEditor() {
		
	}
	#endif
}