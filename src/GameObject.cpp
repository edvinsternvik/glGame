#include "GameObject.h"
#include "Components/ComponentList.h"

namespace glGame {
	GameObject::GameObject(std::string& name) : name(name) {
		init();
	}

	GameObject::~GameObject() {
	}

	void GameObject::onInit() {
		while(m_initQueue.size() > 0) {
			getInitQueueComponent()->init();
		}
	}

	void GameObject::onUpdate() {
		for(auto& c : m_components) {
			c->update();
		}
	}

	void GameObject::onRender() {
		for(auto& c : m_components) {
			c->onRender();
		}
	}

	MeshRenderer* GameObject::addComponentOverload(componentType<MeshRenderer>) {
		if(meshRenderer) return meshRenderer;
		
		meshRenderer = addComponentImpl<MeshRenderer>();
		return meshRenderer;
	}

	Component* GameObject::addComponent(std::string& component) {
		if(component == "Transform") return addComponent<Transform>();
		else if(component == "Camera") return addComponent<Camera>();
		else if(component == "MeshRenderer") return addComponent<MeshRenderer>();
		else if(component == "Script") return addComponent<Script>();
		else {
			std::cout << "ERROR: Could not add component: " << component << " to gameobject: " << name << ". Component " << component << " was not found" << std::endl;
			return nullptr;
		}
	}

	void GameObject::removeComponent(int index) {
		if(index < 1 || index > getComponentSize()) // index < 1 because it should not remove Transform
			return;

		if(m_components[index]->getName() == "MeshRenderer")
			meshRenderer = nullptr;

		m_components.erase(m_components.begin() + index);
	}

	const Component* const GameObject::getComponent(int index) const {
		if(index < 0 || index > getComponentSize())
			return nullptr;

		return m_components[index].get();
	}

	Component* GameObject::getInitQueueComponent() {
		Component* r = m_initQueue.front();
		m_initQueue.pop();
		return r;
	}

	void GameObject::init() {
		transform = addComponent<Transform>();
	}
}
