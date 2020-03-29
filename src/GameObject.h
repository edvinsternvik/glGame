#pragma once
#include <string>
#include <vector>
#include <queue>
#include "Components/Component.h"

namespace glGame{

	class Transform;
	class MeshRenderer;

	class GameObject {
	public:
		GameObject(std::string& name);
		~GameObject();

		void onInit();
		void onUpdate();
		void onRender();

		template<class T>
		T* addComponent() {
			T* newComponent = new T;
			m_components.push_back(newComponent);
			newComponent->setParentGameObject(this);
			m_initQueue.push((Component*)newComponent);

			return newComponent;
		}

		Component* addComponent(std::string& component);

		const Component* const getComponent(int index) const;
		int getComponentSize() const { return m_components.size(); }

	public:
		std::string name;
		Transform* transform = nullptr;
		MeshRenderer* meshRenderer = nullptr;

	private:
		void init();
		Component* getInitQueueComponent();
		int getinitQueueSize() const { return m_initQueue.size(); }

		std::vector<Component*> m_components;
		std::queue<Component*> m_initQueue;
	};

}