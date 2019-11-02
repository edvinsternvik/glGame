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

		void onUpdate();

		template<class T>
		T* addComponent() {
			T* newComponent = new T;
			m_components.push_back(newComponent);
			newComponent->setParentGameObject(this);

			return newComponent;
		}

		const Component* const getComponent(int index) const;
		int getComponentSize() const { return m_components.size(); }

	public:
		std::string name;
		Transform* transform;
		MeshRenderer* meshRenderer;

	private:
		void init();

		std::vector<Component*> m_components;
	};

}