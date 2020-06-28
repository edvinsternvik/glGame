#pragma once
#include "../GameObject.h"

#include <memory>
#include <vector>

namespace glGame {

	class Camera;

	class Scene {
	public:
		Scene();

		std::shared_ptr<GameObject> createGameObject(std::string name);
		std::shared_ptr<GameObject> createGameObject(std::shared_ptr<GameObject> gameObject);
		void deleteGameObject(GameObject* gameObject);
		GameObject* getGameObject(int index);
		void selectGameObject(int index);
		void deselectGameObject() { m_selectedGameObject = std::weak_ptr<GameObject>(); }
		std::shared_ptr<GameObject> getSelectedGameObject() const { return m_selectedGameObject.lock(); }
		int getGameObjectCount() { return m_gameObjects.size(); }

		void init();
		void update(float deltatime);
		void onRender();

	public:
		Camera* activeCamera = nullptr;
		
	private:
		std::vector<std::shared_ptr<GameObject>> m_gameObjects;
		std::weak_ptr<GameObject> m_selectedGameObject = std::weak_ptr<GameObject>(); //Holds a reference to the selected gameobject in the editor
	};

}