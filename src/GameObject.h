#pragma once
#include <string>
#include <vector>
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

		std::vector<Component*> m_components;
	};

}