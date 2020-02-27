#include "SceneManager.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "../Math/Vector.h"

#include "../Components/Transform.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"

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

	void SceneManager::initializeScene() {
		const char* fileName = "Scene0.scene";

		std::ifstream file(fileName);

		if(file.is_open()) {
			loadScene(fileName);
		}
		else {
			if(m_activeScene->activeCamera == nullptr) {
				GameObject* cameraObject = m_activeScene->createGameObject("Camera");
				Camera* cameraComponent = cameraObject->addComponent<Camera>();

				m_activeScene->activeCamera = cameraComponent;
			}	
		}
	}

	void SceneManager::saveActiveScene() {
		const char* fileName = "Scene0.scene";

		std::ofstream file(fileName);

		if(!file.is_open()) {
			std::cout << "Could not open file: " << fileName << std::endl;
			return;
		}

		int gameObjects = m_activeScene->getGameObjectCount();
		for(int i = 0; i < gameObjects; ++i) {
			GameObject* gameObject = m_activeScene->getGameObject(i);

			file << "GameObject: " << gameObject->name << "\n";

			int components = gameObject->getComponentSize();
			for(int j = 0; j < components; ++j) {
				const Component* component = gameObject->getComponent(j);

				file << "Component: " << component->getName() << "\n";

				int publicVariables = component->getPublicVariableCount();
				for(int k = 0; k < publicVariables; ++k) {
					PublicVariable pVar = component->getPublicVariable(k);

					if(pVar.variableType == PublicVariableType::Vec3) {
						Vector3 vec = *((Vector3*)pVar.data);

						file << "Attribute: " << pVar.name << " " << "{" << vec.x << "," << vec.y << "," << vec.z << "}\n";
					}
				}
			}
		}

		file.close();
	}

	void SceneManager::loadScene(const char* fileName) {
		std::ifstream file(fileName);

		if(!file.is_open()) {
			std::cout << "Could not open file: " << fileName << std::endl;
			return;
		}

		GameObject* activeGameObject = nullptr;
		Component* activeComponent = nullptr;

		std::string line;
		while(std::getline(file, line)) {
			std::stringstream linebuffer;
			linebuffer << line;

			std::vector<std::string> strings;
			std::string word;
			while(std::getline(linebuffer, word, ' ')) {
				strings.push_back(word);
			}

			if(strings[0] == "GameObject:") {
				if(strings.size() < 2) {
					std::cout << "Error formating in file: " << fileName << " on line: " << line << std::endl;
					break;
				}

				std::string gameObjectName = strings[1];
				for(int i = 2; i < strings.size(); ++i) gameObjectName += " " + strings[i];

				activeGameObject = m_activeScene->createGameObject(gameObjectName);
			}
			else if(strings[0] == "Component:") {
				if(strings.size() != 2) {
					std::cout << "Error formating in file: " << fileName << " on line: " << line << std::endl;
					break;
				}

				if(activeGameObject == nullptr) {
					std::cout << "Error loading Component in file: " << fileName << ". No GameObject selected" << std::endl;
					break;
				}

				if(strings[1] == "Transform") activeComponent = activeGameObject->transform;
				else if(strings[1] == "Camera") {
					m_activeScene->activeCamera = activeGameObject->addComponent<Camera>();
					activeComponent = (Component*)m_activeScene->activeCamera;
				}
				else if(strings[1] == "MeshRenderer") {
					activeGameObject->meshRenderer = activeGameObject->addComponent<MeshRenderer>();
					activeComponent = (Component*)activeGameObject->meshRenderer;
				}
			}
			else if(strings[0] == "Attribute:") {
				if(strings.size() != 3) {
					std::cout << "Error formating in file: " << fileName << " on line: " << line << std::endl;
					break;
				}

				if(activeComponent == nullptr) {
					std::cout << "Error loading Attribute in file: " << fileName << ". No Component selected" << std::endl;
					break;
				}

				PublicVariable* pVar = activeComponent->getPublicVariable(strings[1].c_str());
				if(pVar != nullptr) {

					switch(pVar->variableType) {
					case PublicVariableType::Vec3:
						Vector3 tmpData(0, 0, 0);

						std::stringstream valueBuffer;
						valueBuffer << strings[2].substr(1, strings[2].size() - 2);
						std::string tmp;
						for(int i = 0; i < 3; ++i) {
							std::getline(valueBuffer, tmp, ',');
							
							((float*)&tmpData)[i] = std::stof(tmp);
						};
						
						*(Vector3*)pVar->data = tmpData;
						break;
					}

					// activeComponent->setPublicVariable()
				}
			}
		}

		file.close();

	}



}