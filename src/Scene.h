#pragma once
#include "GameObject.h"

#include <memory>
#include <vector>

namespace glGame {

	class Scene {
	public:

		GameObject* createGameObject();

		void update();

	private:
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	};

}