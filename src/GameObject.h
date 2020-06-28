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
			return addComponentOverload(componentType<T>());
		}

		std::shared_ptr<Component> addComponent(std::string& component);
		void addComponentToInitQueue(int index);
		void removeComponent(int index);

		const Component* const getComponent(int index) const;
		const Component* const getComponent(const std::string& componentName) const;
		int getComponentSize() const { return m_components.size(); }

	public:
		std::string name;
		Transform* transform = nullptr;
		std::vector<RendererComponent*> rendererComponents;

	private:
		void init();
		Component* getInitQueueComponent();
		int getinitQueueSize() const { return m_initQueue.size(); }

		template<class T> struct componentType {};

		template<class T>
		std::shared_ptr<T> addComponentImpl() {
			std::shared_ptr<T> newComponent = std::make_shared<T>();
			newComponent->setParentGameObject(m_this);
			m_initQueue.push((Component*)newComponent.get());
			m_components.push_back(newComponent);

			return newComponent;
		}

		std::shared_ptr<MeshRenderer> addComponentOverload(componentType<MeshRenderer>);
		std::shared_ptr<LineRenderer> addComponentOverload(componentType<LineRenderer>);
		template<class T>
		std::shared_ptr<T> addComponentOverload(componentType<T>) {
			return addComponentImpl<T>();
		}

		std::vector<std::shared_ptr<Component>> m_components;
		std::queue<Component*> m_initQueue;
		std::weak_ptr<GameObject> m_this;
	};

}