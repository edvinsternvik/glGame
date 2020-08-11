#pragma once
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Input.h"
#include "Resources/SceneManager.h"
#include "Time.h"
#include "Resources/AssetManager.h"
#include "Physics/Physics3d.h"

#ifdef GL_GAME_EDITOR
#include "Editor/Editor.h"
#endif

#include <memory>

namespace glGame {

	class Application {
	public:
		Application();

		void run();

		static Application& Get()  { return *s_Instance; }

	private:
		void onEvent(Event& e);

	public:
		Physics3d physics;
		Renderer renderer;
		std::unique_ptr<SceneManager> sceneManager;

	private:
		bool m_running = true, m_focused = true;
		std::unique_ptr<Window> m_window;
		std::unique_ptr<AssetManager> m_assetManager;
		std::unique_ptr<Time> m_time;
		#ifdef GL_GAME_EDITOR
		std::unique_ptr<Editor> m_editor;
		#endif

		float m_viewportAspectRatio;

	private:
		static Application* s_Instance;
	};
}