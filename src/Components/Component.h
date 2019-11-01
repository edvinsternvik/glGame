#pragma once
#include <string>
#include <vector>

namespace glGame {

	class GameObject;

	enum class EditorVariableType {
		Int=0, Float, Double, Char, String, GameObject, Component, Vec2, Vec3, Asset, Color
	};

	class Component {
	public:
		Component() {}

		virtual std::string getName() const = 0;
		virtual void update() = 0;

		inline const int getEditorVariableCount() const {
			return m_editorVariables.size();
		}

		inline const std::pair<void*, EditorVariableType>& getEditorVariable(int index) const {
			return m_editorVariables[index];
		}

	protected:
		inline void addEditorVariable(void* variable, EditorVariableType varType) {
			m_editorVariables.push_back(std::pair<void*, EditorVariableType>(variable, varType));
		}

		GameObject* gameObject;

	private:
		inline void setParentGameObject(GameObject* parent) {
			gameObject = parent;
		}
		friend class GameObject;

	private:
		std::vector<std::pair<void*, EditorVariableType>> m_editorVariables;
		
	};

}