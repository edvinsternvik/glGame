#pragma once
#include "GameObject.h"

#include <memory>
#include <vector>

namespace glGame {

	class Scene {
	public:

		GameObject* createGameObject(std::string name);
		std::unique_ptr<GameObject> const& getGameObject(int index);
		inline int getGameObjectCount() { return m_gameObjects.size(); }

		void update();

	private:
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	};

}