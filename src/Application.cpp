#include "Application.h"
#include "Components/CameraComponent.h"
#include <iostream>

namespace glGame {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		s_Instance = this;

		std::string title = "glGame";
		Vector2i viewportSize(1280, 720);
		m_window = std::make_unique<Window>(title, viewportSize.x, viewportSize.y);
		m_window->setEventFunction(std::bind(&Application::onEvent, this, std::placeholders::_1));
		Input::m_window = m_window.get();

		renderer.init(viewportSize); // Initializes glew, has to be called before any opengl code

		m_assetManager = std::make_unique<AssetManager>();

		sceneManager = std::make_unique<SceneManager>();
		sceneManager->initializeScene();

		#ifdef GL_GAME_EDITOR
		m_editor = std::make_unique<Editor>(std::bind(&Application::onEvent, this, std::placeholders::_1), m_window.get(), viewportSize);
		#endif
	}

	void Application::run() {
		#ifdef GL_GAME_EDITOR
		m_editor->setActiveScene(sceneManager->getActiveScene());
		m_editor->setAssetManager(m_assetManager.get());
		#endif

		m_time = std::make_unique<Time>();

		while(m_running) {
			sceneManager->initScene();

			float deltatime = m_time->getDeltatime();
			sceneManager->updateScene(deltatime);
			#ifndef GL_GAME_EDITOR
				physics.stepSimulation(deltatime);
			#else
				m_editor->update(deltatime, m_focused);
			#endif

			sceneManager->callSceneOnRender();

			renderer.render(sceneManager->getActiveScene(), &sceneManager->getActiveScene()->activeCamera.lock()->camera);

			#ifdef GL_GAME_EDITOR
			m_editor->renderEditor();
			#endif

			Input::Update();
			m_window->pollEvents();
			m_window->swapBuffers();
		}
	}

	void Application::onEvent(Event& e) {
		if(e.isInCategory(EventCategory::Input)) {
			Input::HandleEvent(&e);
		}

		else if(e.isInCategory(EventCategory::Application)) {
			if(e.getEventType() == EventType::WindowClose) {
				m_running = false;
			}
		}

		else if(e.isInCategory(EventCategory::Editor)) {
			if(e.getEventType() == EventType::SaveScene) {
				std::cout << "Scene saved" << std::endl;
				sceneManager->saveActiveScene();
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