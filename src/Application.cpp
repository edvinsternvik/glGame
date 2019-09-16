#include "Application.h"
#include <iostream>

namespace glGame {

	Application::Application() {
		std::string title = "glGame";
		int width = 1280, height = 720;
		m_window = std::make_unique<Window>(title, width, height);
		m_window->setEventFunction(std::bind(&Application::onEvent, this, std::placeholders::_1));
		m_renderer = std::make_unique<Renderer>();

		m_scene = std::make_shared<Scene>();
		m_gui = std::make_unique<Gui>(m_window->getWindow());
		m_gui->m_windows.push_back(std::make_unique<ViewportWindow>(m_renderer->getFrameTexture(), width / (float)height));
		m_gui->m_windows.push_back(std::make_unique<SceneWindow>(m_scene));
	}

	void Application::run() {
		while(m_running) {

			m_scene->update();

			m_renderer->render();
			m_gui->OnGuiRender();

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
	}
}