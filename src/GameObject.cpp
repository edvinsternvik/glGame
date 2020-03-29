#include "GameObject.h"
#include "Components/ComponentList.h"

namespace glGame {
	GameObject::GameObject(std::string& name) : name(name) {
		init();
	}

	GameObject::~GameObject() {
		for(Component* cmp : m_components) {
			delete cmp;
		}
	}

	void GameObject::onInit() {
		while(m_initQueue.size() > 0) {
			getInitQueueComponent()->init();
		}
	}

	void GameObject::onUpdate() {
		for(Component* c : m_components) {
			c->update();
		}
	}

	void GameObject::onRender() {
		for (Component* c : m_components) {
			c->onRender();
		}
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

	const Component* const GameObject::getComponent(int index) const {
		if(index < 0 || index > getComponentSize())
			return nullptr;

		return m_components[index];
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
