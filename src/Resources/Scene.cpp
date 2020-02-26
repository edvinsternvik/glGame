#include "Scene.h"

#include "../Components/Camera.h"
#include "../Components/Transform.h"

namespace glGame {

	Scene::Scene() {
		GameObject* cameraObject = createGameObject("Camera");
		Camera* cameraComponent = cameraObject->addComponent<Camera>();

		activeCamera = cameraComponent;
	}

	Scene::~Scene() {
		for (GameObject* go : m_gameObjects) {
			delete go;
		}
	}

	GameObject* Scene::createGameObject(std::string name) {
		GameObject* newGameObject = new GameObject(name);
		m_gameObjects.push_back(newGameObject);
		return newGameObject;
	}

	GameObject* Scene::getGameObject(int index) {
		if (index < 0 || index > m_gameObjects.size()) {
			return nullptr;
		}

		return m_gameObjects[index];
	}

	void Scene::selectGameObject(int index) {
		m_selectedGameObject = getGameObject(index);
	}

	void Scene::update() {
		for (auto& go : m_gameObjects) {
			go->onUpdate();
		}
	}

	void Scene::onRender() {
		for (auto& go : m_gameObjects) {
			go->onRender();
		}
	}
}
