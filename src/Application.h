#pragma once
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Input.h"

#include <memory>

namespace glGame {

	class Application {
	public:
		Application();

		void run();

	private:
		void onEvent(Event& e);

	private:
		bool m_running = true;
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;
	};
}