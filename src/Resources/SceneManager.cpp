#include "SceneManager.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "../Math/Vector.h"

#include "../Components/ComponentList.h"

namespace glGame {

	SceneManager::SceneManager() {
		addScene();
		setActiveScene(0);
	}

	void SceneManager::initScene() {
		m_activeScene->init();
	}

	void SceneManager::updateScene(float deltatime) {
		m_activeScene->update(deltatime);
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
		bool fileOpen = file.is_open();
		file.close();

		if(fileOpen) {
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
					
					file << "Attribute: " << pVar.name << " " << getPublicVariableString(pVar) << "\n";
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

				activeComponent = addComponent(strings[1], activeGameObject);
			}
			else if(strings[0] == "Attribute:") {
				if(strings.size() < 3) {
					std::cout << "Error formating in file: " << fileName << " on line: " << line << std::endl;
					break;
				}

				if(activeComponent == nullptr) {
					std::cout << "Error loading Attribute in file: " << fileName << ". No Component selected" << std::endl;
					break;
				}

				std::string pVarName = strings[1]; // Combine all the strings except the last one
				for(int i = 2; i < strings.size() - 1; ++i) pVarName += strings[i];

				PublicVariable* pVar = activeComponent->getPublicVariable(strings[1]);
				if(pVar != nullptr) {
					pVar->setData(strings[strings.size() - 1]);
				}
				else if(activeComponent->getName() == "Script") {
					((Script*)activeComponent)->registerPublicScriptVariable(strings);
				}
			}
		}

		file.close();

	}

	std::string SceneManager::getPublicVariableString(PublicVariable& pVar) {
		switch(pVar.variableType) {
		case PublicVariableType::Int: return std::to_string(*((int*)pVar.data));
		case PublicVariableType::Float: return std::to_string(*((float*)pVar.data));
		case PublicVariableType::Double: return std::to_string(*((double*)pVar.data));
		case PublicVariableType::Char: return std::to_string(*((char*)pVar.data));
		case PublicVariableType::String: return *(std::string*)pVar.data;
		case PublicVariableType::GameObject: return "";
		case PublicVariableType::Component: return "";
		case PublicVariableType::Vec2: {
			Vector2 vec2 = *((Vector2*)pVar.data);
			return "{" + std::to_string(vec2.x) + "," + std::to_string(vec2.y) + "}";
		}
		case PublicVariableType::Vec3: {
			Vector3 vec3 = *((Vector3*)pVar.data);
			return "{" + std::to_string(vec3.x) + "," + std::to_string(vec3.y) + "," + std::to_string(vec3.z) + "}";
		}
		case PublicVariableType::Color: return "";
		case PublicVariableType::Model: return std::to_string(*(unsigned int*)pVar.data);
		}
		return "";
	}

	Component* SceneManager::addComponent(std::string& component, GameObject* gameObject) {
		if(component == "Transform") return gameObject->transform;
		else if(component == "Camera") {
			m_activeScene->activeCamera = gameObject->addComponent<Camera>();
			return (Component*)m_activeScene->activeCamera;
		}
		else if(component == "MeshRenderer") return (Component*)gameObject->addComponent<MeshRenderer>();
		else if(component == "Script") return (Component*)gameObject->addComponent<Script>();
		else if(component == "RigidBody") return (Component*)gameObject->addComponent<RigidBody>();

		return nullptr;
	}
}
