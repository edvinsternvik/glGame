#pragma once
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"

#ifdef GL_GAME_EDITOR
#include "Editor/Gui.h"
#endif

#include <memory>

namespace glGame {

	class Application {
	public:
		Application();

		void run();

	private:
		void onEvent(Event& e);
		void setUpEditor();

	private:
		bool m_running = true;
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_scene;
		
		#ifdef GL_GAME_EDITOR
		std::unique_ptr<Gui> m_editorGui;
		#endif

		float m_viewportAspectRatio;
	};
}