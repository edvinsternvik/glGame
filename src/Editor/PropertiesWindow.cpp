#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Scene.h"

namespace glGame {

	PropertiesWindow::PropertiesWindow(std::shared_ptr<Scene>* scene) : m_scene(scene) {
	}

	void PropertiesWindow::renderWindow() {
		GameObject* selectedObj = (*m_scene)->getSelectedGameObject();

		if (!selectedObj) {
			return;
		}

		std::string msg = selectedObj->name;
		msg.resize(128);
		char* test = (char*)msg.c_str();
		if (ImGui::InputText("###NAME", test, msg.size())) {
			selectedObj->name = std::string(test);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		int componentSize = selectedObj->getComponentSize();
		for(int i = 0; i < componentSize; ++i) {
			const Component* selectedComponent = selectedObj->getComponent(i);
			ImGui::Text(selectedComponent->getName().c_str());

			int editorVariableCount = selectedComponent->getEditorVariableCount();
			for(int editorVariableIndex = 0; editorVariableIndex < editorVariableCount; editorVariableIndex++) {
				const std::pair<void*, EditorVariableType>* editorVariable = &selectedComponent->getEditorVariable(editorVariableIndex); 
				drawComponentVariableGui(editorVariable->first, editorVariable->second);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
		}
	}

	void PropertiesWindow::drawComponentVariableGui(void* data, EditorVariableType varType) {
		switch(varType) {

		case EditorVariableType::Int:
			ImGui::InputInt("Test int", (int*)data);
			break;
		case EditorVariableType::Float:
			ImGui::InputFloat("Test float", (float*)data);
			break;
		case EditorVariableType::Double:
			ImGui::InputDouble("Test double", (double*)data);
			break;

		}
	}

}