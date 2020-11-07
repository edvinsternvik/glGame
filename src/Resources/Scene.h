#pragma once
#include "../GameObject.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace glGame {

	class CameraComponent;

	class Scene {
	public:
		Scene();

		std::shared_ptr<GameObject> createGameObject(std::string name);
		std::shared_ptr<GameObject> createGameObject(std::shared_ptr<GameObject> gameObject);
		void deleteGameObject(GameObject* gameObject);
		std::weak_ptr<GameObject> getGameObjectByIndex(int index);
		std::weak_ptr<GameObject> getGameObject(int id);
		int getGameObjectCount() { return m_gameObjects.size(); }

		void init();
		void update(float deltatime);
		void onRender();

	public:
		std::weak_ptr<CameraComponent> activeCamera;
		
	private:
		std::vector<std::shared_ptr<GameObject>> m_gameObjects;
		std::unordered_map<int, std::weak_ptr<GameObject>> m_gameObjectIdMap;
		int nextId = 0;
	};

}