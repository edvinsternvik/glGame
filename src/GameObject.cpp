#include "GameObject.h"
#include "Components/Transform.h"

#include "Components/MeshRenderer.h"

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
	
	template<class T>
	void GameObject::addComponent() {
		T* newComponent = new T;
		m_components.push_back(newComponent);
		newComponent->setParentGameObject(this);
	}

	const Component* const GameObject::getComponent(int index) const {
		if(index < 0 || index > getComponentSize())
			return nullptr;

		return m_components[index];
	}

	void GameObject::init() {
		addComponent<Transform>();
		transform = (Transform*)m_components[0];

		addComponent<MeshRenderer>();
		meshRenderer = (MeshRenderer*)m_components[1];
	}

}
