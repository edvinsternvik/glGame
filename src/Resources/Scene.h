#pragma once
#include "../GameObject.h"

#include <memory>
#include <vector>

namespace glGame {

	class CameraComponent;

	class Scene {
	public:
		Scene();

		std::shared_ptr<GameObject> createGameObject(std::string name);
		std::shared_ptr<GameObject> createGameObject(std::shared_ptr<GameObject> gameObject);
		void deleteGameObject(GameObject* gameObject);
		std::weak_ptr<GameObject> getGameObject(int index);
		std::shared_ptr<GameObject> getGameObjectShared(int index);
		int getGameObjectCount() { return m_gameObjects.size(); }

		void init();
		void update(float deltatime);
		void onRender();

	public:
		std::weak_ptr<CameraComponent> activeCamera;
		
	private:
		std::vector<std::shared_ptr<GameObject>> m_gameObjects;
	};

}