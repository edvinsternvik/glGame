#pragma once
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Gui.h"

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
		std::unique_ptr<Scene> m_scene;
		std::unique_ptr<Gui> m_gui;
	};
}