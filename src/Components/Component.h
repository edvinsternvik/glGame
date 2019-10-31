#pragma once
#include <string>
#include <vector>

namespace glGame {

	enum class EditorVariableType {
		Int=0, Float, Double, Char, String, GameObject, Component, Vec2, Vec3, Asset
	};

	class Component {
	public:
		Component() {}

		virtual std::string getName() const = 0;
		virtual void update() = 0;

		inline const int getEditorVariableCount() const {
			return editorVariables.size();
		}

		inline const std::pair<void*, EditorVariableType>& getEditorVariable(int index) const {
			return editorVariables[index];
		}

	protected:
		inline void addEditorVariable(void* variable, EditorVariableType varType) {
			editorVariables.push_back(std::pair<void*, EditorVariableType>(variable, varType));
		}

	private:
		std::vector<std::pair<void*, EditorVariableType>> editorVariables;
	};

}