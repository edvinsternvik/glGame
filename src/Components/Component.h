#pragma once
#include <vector>
#include <iostream>
#include "PublicVariable.h"

namespace glGame {

	class GameObject;

	class Component {
	public:
		Component() {}
		virtual ~Component() {}

		virtual std::string getName() const = 0;
		virtual void init() {}
		virtual void update() = 0;
		virtual void onRender() {}

		inline const int getPublicVariableCount() const {
			return m_publicVariables.size();
		}

		inline const PublicVariable& getPublicVariable(int index) const {
			return m_publicVariables[index];
		}

		inline PublicVariable* const getPublicVariable(const std::string& name) {
			for(PublicVariable& pVar : m_publicVariables) {
				if(name == pVar.name) return &pVar;
			}

			return nullptr;
		}

		const GameObject* const getGameObject() const { return gameObject; }

	protected:
		inline void addPublicVariable(void* variable, PublicVariableType varType, std::string name) {
			m_publicVariables.push_back(PublicVariable(variable, varType, name));
		}

		inline void clearPublicVariables() { m_publicVariables.clear(); }
		void removePublicVariable(int index) {
			if(index >= 0 && index < m_publicVariables.size()) m_publicVariables.erase(m_publicVariables.begin() + index);
		}

		inline void addPublicVariable(void* variable, PublicVariableType varType, const char* name, float sliderSpeed) {
			m_publicVariables.push_back(PublicVariable(variable, varType, name, sliderSpeed));
		}

		GameObject* gameObject = nullptr;

	private:
		inline void setParentGameObject(GameObject* parent) {
			gameObject = parent;
		}
		friend class GameObject;

	private:
		std::vector<PublicVariable> m_publicVariables;
		
	};

}