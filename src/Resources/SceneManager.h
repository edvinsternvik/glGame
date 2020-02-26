#pragma once
#include <vector>
#include <memory>

#include "Scene.h"

namespace glGame {

	class SceneManager {
	public:
		SceneManager();

		void updateScene();
		void addScene();
		void setActiveScene(int index);
		Scene* getActiveScene() const { return m_activeScene; };

	private:
		std::vector<std::unique_ptr<Scene>> m_scenes;
		Scene* m_activeScene = nullptr;
	};

}