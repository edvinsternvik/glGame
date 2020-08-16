#pragma once
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include "Components/Component.h"

namespace glGame{

	class Transform;
	class RendererComponent;
	class MeshRenderer;
	class LineRenderer;

	class GameObject {
	private:
		GameObject(const std::string& name);

	public:
		static std::shared_ptr<GameObject> Create(const std::string& name);
		~GameObject();

		void onInit();
		void onUpdate(float deltatime);
		void onRender();

		template<class T>
		std::shared_ptr<T> addComponent() {
			std::shared_ptr<T> newComponent = std::make_shared<T>();
			newComponent->setParentGameObject(m_this);
			m_initQueue.push((Component*)newComponent.get());
			m_components.push_back(newComponent);

			return newComponent;
		}

		std::shared_ptr<Component> addComponent(std::shared_ptr<Component> component);
		std::shared_ptr<Component> addComponent(std::string& component);

		void addComponentToInitQueue(int index);
		void addComponentToInitQueue(Component* component);
		void removeComponent(int index);
		void removeComponent(Component* component);

		template<class T>
		std::weak_ptr<T> getComponent() {
			for(auto& component : m_components) {
				if(std::shared_ptr<T> c = std::dynamic_pointer_cast<T>(component)) {
					return c;
				}
			}
			return std::weak_ptr<T>();
		}
		const std::shared_ptr<Component> getComponent(int index) const;
		const Component* const getComponent(const std::string& componentName) const;
		int getComponentSize() const { return m_components.size(); }

	public:
		std::string name;
		Transform* transform = nullptr;

	private:
		void init();
		Component* getInitQueueComponent();
		int getinitQueueSize() const { return m_initQueue.size(); }

		std::vector<std::shared_ptr<Component>> m_components;
		std::queue<Component*> m_initQueue;
		std::weak_ptr<GameObject> m_this;
	};

}