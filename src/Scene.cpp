#include "Scene.h"

namespace glGame {

	GameObject* Scene::createGameObject() {
		m_gameObjects.push_back(std::make_unique<GameObject>());
		return m_gameObjects.back().get();
	}

	void Scene::update() {
		for (auto& go : m_gameObjects) {
			go->onUpdate();
		}
	}
}
