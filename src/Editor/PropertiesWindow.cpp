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

		ImGui::Spacing();

		std::string msg = selectedObj->name;
		msg.resize(128);
		char* test = (char*)msg.c_str();
		if (ImGui::InputText("###NAME", test, msg.size())) {
			selectedObj->name = std::string(test);
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("Components");
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Spacing();

		int componentSize = selectedObj->getComponentSize(), componentVariableIndex = 0;
		for(int i = 0; i < componentSize; ++i) {
			const Component* selectedComponent = selectedObj->getComponent(i);
			ImGui::Text(selectedComponent->getName().c_str());

			ImGui::Spacing();

			int editorVariableCount = selectedComponent->getEditorVariableCount();
			for(int editorVariableIndex = 0; editorVariableIndex < editorVariableCount; ++editorVariableIndex) {
				const EditorVariable* editorVariable = &selectedComponent->getEditorVariable(editorVariableIndex);

				ImGui::PushID(componentVariableIndex++); 
				drawComponentVariableGui(editorVariable);
				ImGui::PopID();

				ImGui::Spacing();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
		}
	}

	void PropertiesWindow::drawComponentVariableGui(const EditorVariable* editorVariable) {
		float speedMultiplier = 0.01f;
		switch(editorVariable->variableType) {

		case EditorVariableType::Int:
			ImGui::DragInt(editorVariable->name, (int*)editorVariable->data, editorVariable->sliderSpeed * speedMultiplier);
			break;
		case EditorVariableType::Float:
			ImGui::DragFloat(editorVariable->name, (float*)editorVariable->data, editorVariable->sliderSpeed * speedMultiplier);
			break;
		case EditorVariableType::Double:
			ImGui::InputDouble(editorVariable->name, (double*)editorVariable->data);
			break;
		case EditorVariableType::Vec2:
			ImGui::DragFloat2(editorVariable->name, (float*)editorVariable->data, editorVariable->sliderSpeed * speedMultiplier);
			break;
		case EditorVariableType::Vec3:
			ImGui::DragFloat3(editorVariable->name, (float*)editorVariable->data, editorVariable->sliderSpeed * speedMultiplier);
			break;
		case EditorVariableType::String:
			ImGui::InputText(editorVariable->name, (char*)editorVariable->data, 256);
			break;
		case EditorVariableType::Color:
			ImGui::ColorPicker3(editorVariable->name, (float*)editorVariable->data);
			break;
		}
	}

}