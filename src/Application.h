#pragma once
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"

#include <memory>

//test
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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
	};
}