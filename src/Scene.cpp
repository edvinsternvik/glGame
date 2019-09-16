#include "Scene.h"

namespace glGame {

	GameObject* Scene::createGameObject(std::string name) {
		m_gameObjects.push_back(std::make_unique<GameObject>(name));
		return m_gameObjects.back().get();
	}

	std::unique_ptr<GameObject> const& Scene::getGameObject(int index) {
		if (index < 0 || index > m_gameObjects.size()) {
			return nullptr;
		}

		return m_gameObjects[index];
	}

	void Scene::update() {
		for (auto& go : m_gameObjects) {
			go->onUpdate();
		}
	}
}
