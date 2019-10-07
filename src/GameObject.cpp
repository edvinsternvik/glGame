#include "GameObject.h"
#include "Components/Transform.h"

namespace glGame {
	GameObject::GameObject(std::string& name) : name(name) {
		init();
	}

	GameObject::~GameObject() {
		for(Component* cmp : m_components) {
			delete cmp;
		}
	}

	void glGame::GameObject::onUpdate() {
		for(Component* c : m_components) {
			c->update();
		}
	}

	void GameObject::addComponent(Component* component) {
		m_components.push_back(component);
	}

	const Component* const GameObject::getComponent(int index) const {
		if(index < 0 || index > getComponentSize())
			return nullptr;

		return m_components[index];
	}

	void GameObject::init() {
		m_components.push_back(new Transform);
		transform = m_components[0];
	}

}
