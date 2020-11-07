#include "Scene.h"

#include "../Components/Transform.h"

namespace glGame {

	Scene::Scene() {
	}

	std::shared_ptr<GameObject> Scene::createGameObject(std::string name) {
		std::shared_ptr<GameObject> newGameObject = GameObject::Create(name, nextId++);
		m_gameObjects.push_back(newGameObject);
		m_gameObjectIdMap.insert(std::pair<int, std::weak_ptr<GameObject>>(newGameObject->id, newGameObject));
		return newGameObject;
	}

	std::shared_ptr<GameObject> Scene::createGameObject(std::shared_ptr<GameObject> gameObject) {
		if(gameObject.get() == nullptr) return std::shared_ptr<GameObject>();
		auto search = m_gameObjectIdMap.find(gameObject->id);
		if(search != m_gameObjectIdMap.end()) {
			return gameObject;
		}
		m_gameObjects.push_back(gameObject);
		m_gameObjectIdMap.insert(std::pair<int, std::weak_ptr<GameObject>>(gameObject->id, gameObject));
		return gameObject;
	}

	void Scene::deleteGameObject(GameObject* gameObject) {
		for(int i = 0; i < m_gameObjects.size(); ++i) {
			if(m_gameObjects[i].get() == gameObject) {
				m_gameObjectIdMap.erase(m_gameObjects[i]->id);
				m_gameObjects.erase(m_gameObjects.begin() + i);
				return;
			}
		}
	}

	std::weak_ptr<GameObject> Scene::getGameObjectByIndex(int index) {
		if (index < 0 || index > m_gameObjects.size()) {
			return std::weak_ptr<GameObject>();
		}

		return m_gameObjects[index];
	}

	std::weak_ptr<GameObject> Scene::getGameObject(int id) {
		auto search = m_gameObjectIdMap.find(id);
		if(search == m_gameObjectIdMap.end()) {
			return std::weak_ptr<GameObject>();
		}
		return search->second;
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
