#pragma once
#include "GameObject.h"

#include <memory>
#include <vector>

namespace glGame {

	class Scene {
	public:
		~Scene();

		GameObject* createGameObject(std::string name);
		GameObject* getGameObject(int index);
		void selectGameObject(int index);
		inline void unselectGameObject() { m_selectedGameObject = nullptr; }
		inline GameObject* getSelectedGameObject() const { return m_selectedGameObject; }
		inline int getGameObjectCount() { return m_gameObjects.size(); }

		void update();

	private:
		std::vector<GameObject*> m_gameObjects;
		GameObject* m_selectedGameObject = nullptr; //Holds a reference to the selected gameobject in the editor
	};

}