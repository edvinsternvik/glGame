#include "SceneManager.h"

namespace glGame {

	SceneManager::SceneManager() {
		addScene();
		setActiveScene(0);
	}

	void SceneManager::updateScene() {
		m_activeScene->update();
	}

	void SceneManager::addScene() {
		m_scenes.push_back(std::make_unique<Scene>());
	}

	void SceneManager::setActiveScene(int index) {
		if(index < 0 || index >= m_scenes.size()) {
			return;
		}

		m_activeScene = m_scenes[index].get();
	}


}