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
	public:
		GameObject(const std::string& name);
		~GameObject();

		void onInit();
		void onUpdate(float deltatime);
		void onRender();

		template<class T>
		T* addComponent() {
			return addComponentOverload(componentType<T>());
		}

		Component* addComponent(std::string& component);
		void addComponentToInitQueue(int index);
		void removeComponent(int index);

		const Component* const getComponent(int index) const;
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
		T* addComponentImpl() {
			std::unique_ptr<T> newComponent = std::make_unique<T>();
			newComponent->setParentGameObject(this);
			m_initQueue.push((Component*)newComponent.get());
			T* newComponentPtr = newComponent.get();
			m_components.push_back(std::move(newComponent));

			return newComponentPtr;
		}

		MeshRenderer* addComponentOverload(componentType<MeshRenderer>);
		LineRenderer* addComponentOverload(componentType<LineRenderer>);
		template<class T>
		T* addComponentOverload(componentType<T>) {
			return addComponentImpl<T>();
		}

		std::vector<std::unique_ptr<Component>> m_components;
		std::queue<Component*> m_initQueue;
	};

}