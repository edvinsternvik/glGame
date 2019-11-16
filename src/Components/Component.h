#pragma once
#include <string>
#include <vector>

namespace glGame {

	class GameObject;

	enum class EditorVariableType {
		Int=0, Float, Double, Char, String, GameObject, Component, Vec2, Vec3, Asset, Color
	};

	struct EditorVariable {
		void* data;
		EditorVariableType variableType;
		const char* name;
		float sliderSpeed;

		EditorVariable(void* data, EditorVariableType variableType, const char* name) : data(data), variableType(variableType), name(name), sliderSpeed(1.0f) {}
		EditorVariable(void* data, EditorVariableType variableType, const char* name, float sliderSpeed) : data(data), variableType(variableType), name(name), sliderSpeed(sliderSpeed) {}
	};

	class Component {
	public:
		Component() {}

		virtual std::string getName() const = 0;
		virtual void update() = 0;
		virtual void onRender() {}

		inline const int getEditorVariableCount() const {
			return m_editorVariables.size();
		}

		inline const EditorVariable& getEditorVariable(int index) const {
			return m_editorVariables[index];
		}

	protected:
		inline void addEditorVariable(void* variable, EditorVariableType varType, const char* name) {
			m_editorVariables.push_back(EditorVariable(variable, varType, name));
		}

		inline void addEditorVariable(void* variable, EditorVariableType varType, const char* name, float sliderSpeed) {
			m_editorVariables.push_back(EditorVariable(variable, varType, name, sliderSpeed));
		}

		GameObject* gameObject = nullptr;

	private:
		inline void setParentGameObject(GameObject* parent) {
			gameObject = parent;
		}
		friend class GameObject;

	private:
		std::vector<EditorVariable> m_editorVariables;
		
	};

}