#include "Application.h"
#include <iostream>

namespace glGame {

	Application::Application() {
		std::string title = "glGame";
		m_window = std::make_unique<Window>(title, 1280, 720);
		m_window->setEventFunction(std::bind(&Application::onEvent, this, std::placeholders::_1));
		m_renderer = std::make_unique<Renderer>();
	}

	void Application::run() {
		while(m_running) {

			m_renderer->render();

			Input::update();
			m_window->swapBuffers();
			m_window->pollEvents();
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