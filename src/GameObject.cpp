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

	void glGame::GameObject::onRender() {
		for (Component* c : m_components) {
			c->onRender();
		}
	}

	const Component* const GameObject::getComponent(int index) const {
		if(index < 0 || index > getComponentSize())
			return nullptr;

		return m_components[index];
	}

	void GameObject::init() {
		transform = addComponent<Transform>();
	}

}
