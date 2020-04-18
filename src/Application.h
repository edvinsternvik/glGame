#pragma once
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Input.h"
#include "Resources/SceneManager.h"
#include "Time.h"

#ifdef GL_GAME_EDITOR
#include "Editor/Editor.h"
#endif

#include <memory>

namespace glGame {

	class Application {
	public:
		Application();

		void run();

	private:
		void onEvent(Event& e);

	private:
		bool m_running = true, m_focused = true;
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<SceneManager> m_sceneManager;
		Time m_time;
		#ifdef GL_GAME_EDITOR
		std::unique_ptr<Editor> m_editor;
		#endif

		float m_viewportAspectRatio;
	};
}