#include "Scene.h"

#include "../Components/Transform.h"

namespace glGame {

	Scene::Scene() {
	}

	GameObject* Scene::createGameObject(std::string name) {
		std::shared_ptr<GameObject> newGameObject = GameObject::Create(name);
		m_gameObjects.push_back(newGameObject);
		return newGameObject.get();
	}

	GameObject* Scene::getGameObject(int index) {
		if (index < 0 || index > m_gameObjects.size()) {
			return nullptr;
		}

		return m_gameObjects[index].get();
	}

	void Scene::selectGameObject(int index) {
		if (index < 0 || index > m_gameObjects.size()) {
			return;
		}

		m_selectedGameObject = m_gameObjects[index];
	}

	void Scene::init() {
		for (auto& go : m_gameObjects) {
			go->onInit();
		}
	}

	void Scene::update(float deltatime) {
		for (auto& go : m_gameObjects) {
			go->onUpdate(deltatime);
		}
	}

	void Scene::onRender() {
		for (auto& go : m_gameObjects) {
			go->onRender();
		}
	}
}
