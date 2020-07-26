#include "GameObject.h"
#include "Components/ComponentList.h"

namespace glGame {

	GameObject::GameObject(const std::string& name) : name(name) {
		init();
	}

	std::shared_ptr<GameObject> GameObject::Create(const std::string& name) {
		struct MakeSharedEnabler : public GameObject { // This is needed to use make_shared, as GameObject has a private constructor
			MakeSharedEnabler(const std::string& name) : GameObject(name) {}
		};

		std::shared_ptr<GameObject> newGameObject = std::make_shared<MakeSharedEnabler>(name);
		newGameObject->m_this = newGameObject;
		return newGameObject;
	}

	GameObject::~GameObject() {
		for(auto& c : m_components) c->onDestroy();
	}

	void GameObject::onInit() {
		while(m_initQueue.size() > 0) {
			getInitQueueComponent()->init();
		}
	}

	void GameObject::onUpdate(float deltatime) {
		for(auto& c : m_components) {
			c->update(deltatime);
		}
	}

	void GameObject::onRender() {
		for(auto& c : m_components) {
			c->onRender();
		}
	}

	std::shared_ptr<Component> GameObject::addComponent(std::shared_ptr<Component> component) {
		for(int i = 0; i < m_components.size(); ++i) {
			if(m_components[i].get() == component.get()) return m_components[i];
		}

		m_components.push_back(component);
		addComponentToInitQueue(component.get());
		return component;
	}

	std::shared_ptr<Component> GameObject::addComponent(std::string& component) {
		if(component == "Transform") return addComponent<Transform>();
		else if(component == "Camera") return addComponent<Camera>();
		else if(component == "MeshRenderer") return addComponent<MeshRenderer>();
		else if(component == "LineRenderer") return addComponent<LineRenderer>();
		else if(component == "Script") return addComponent<ScriptComponent>();
		else if(component == "RigidBody") return addComponent<RigidBody>();
		else if(component == "BoxCollider") return addComponent<BoxCollider>();
		else if(component == "Material") return addComponent<MaterialComponent>();
		else if(component == "Skybox") return addComponent<Skybox>();
		else if(component == "Light") return addComponent<LightComponent>();
		else {
			std::cout << "ERROR: Could not add component: " << component << " to gameobject: " << name << ". Component " << component << " was not found" << std::endl;
			return nullptr;
		}
	}

	void GameObject::addComponentToInitQueue(int index) {
		Component* c = (Component*)getComponent(index).get();

		m_initQueue.push(c);
	}

	void GameObject::addComponentToInitQueue(Component* component) {
		m_initQueue.push(component);
	}

	void GameObject::removeComponent(int index) {
		if(index < 1 || index > getComponentSize()) // index < 1 because it should not remove Transform
			return;

		m_components[index]->onDestroy();
		m_components.erase(m_components.begin() + index);
	}

	void GameObject::removeComponent(Component* component) {
		for(int i = 0; i < m_components.size(); ++i) {
			if(m_components[i].get() == component) {
				removeComponent(i);
				return;
			}
		}
	}

	const std::shared_ptr<Component> GameObject::getComponent(int index) const {
		if(index < 0 || index > getComponentSize())
			return nullptr;

		return m_components[index];
	}

	const Component* const GameObject::getComponent(const std::string& componentName) const {
		for(int i = 0; i < getComponentSize(); ++i) {
			if(m_components[i] && m_components[i]->getName() == componentName) return m_components[i].get();
		}
		return nullptr;
	}

	Component* GameObject::getInitQueueComponent() {
		Component* r = m_initQueue.front();
		m_initQueue.pop();
		return r;
	}

	void GameObject::init() {
		transform = addComponent<Transform>().get();
	}
}
