#pragma once
#include <string>
#include <vector>
#include "Components/Component.h"

namespace glGame{

	class GameObject {
	public:
		GameObject(std::string& name);
		~GameObject();

		void onUpdate();

		void addComponent(Component* component);
		const Component* const getComponent(int index) const;
		int getComponentSize() const { return m_components.size(); }

	public:
		std::string name;
		Component* transform;

	private:
		void init();

		std::vector<Component*> m_components;
	};

}