#pragma once
#include <vector>
#include <memory>
#include "PublicVariable.h"

namespace glGame {

	class GameObject;

	class Component {
	public:
		Component() {}
		virtual ~Component() {}

		virtual std::string getName() const = 0;
		virtual void init() {}
		virtual void update(float deltatime) {}
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

		GameObject* const getGameObject() const { return gameObject.lock().get(); }

	protected:
		void addPublicVariable(PublicVarVariant data, std::string name) { m_publicVariables.push_back(PublicVariable(data, name)); }
		void addPublicVariable(PublicVarVariant data, std::string name, const float& sliderSpeed) { m_publicVariables.push_back(PublicVariable(data, name, sliderSpeed)); }
		void addPublicVariable(PublicVariable publicVariable) { m_publicVariables.push_back(publicVariable); }

		inline void clearPublicVariables() { m_publicVariables.clear(); }
		void removePublicVariable(int index) {
			if(index >= 0 && index < m_publicVariables.size()) m_publicVariables.erase(m_publicVariables.begin() + index);
		}


	private:
		inline void setParentGameObject(std::weak_ptr<GameObject> parent) {
			gameObject = parent;
		}
		friend class GameObject;

	private:
		std::weak_ptr<GameObject> gameObject;
		std::vector<PublicVariable> m_publicVariables;
	};

}