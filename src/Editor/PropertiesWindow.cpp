#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Resources/Scene.h"

namespace glGame {

	PropertiesWindow::PropertiesWindow(Scene* scene) : m_scene(scene) {
	}

	void PropertiesWindow::renderWindow() {
		GameObject* selectedObj = m_scene->getSelectedGameObject();

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

			int editorVariableCount = selectedComponent->getPublicVariableCount();
			for(int editorVariableIndex = 0; editorVariableIndex < editorVariableCount; ++editorVariableIndex) {
				const PublicVariable* publicVariable = &selectedComponent->getPublicVariable(editorVariableIndex);

				ImGui::PushID(componentVariableIndex++); 
				drawComponentVariableGui(publicVariable);
				ImGui::PopID();

				ImGui::Spacing();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
		}
	}

	void PropertiesWindow::drawComponentVariableGui(const PublicVariable* editorVariable) {
		float speedMultiplier = 0.01f;
		switch(editorVariable->variableType) {

		case PublicVariableType::Int:
			ImGui::DragInt(editorVariable->name, (int*)editorVariable->data, editorVariable->editor_sliderSpeed * speedMultiplier);
			break;
		case PublicVariableType::Float:
			ImGui::DragFloat(editorVariable->name, (float*)editorVariable->data, editorVariable->editor_sliderSpeed * speedMultiplier);
			break;
		case PublicVariableType::Double:
			ImGui::InputDouble(editorVariable->name, (double*)editorVariable->data);
			break;
		case PublicVariableType::Vec2:
			ImGui::DragFloat2(editorVariable->name, (float*)editorVariable->data, editorVariable->editor_sliderSpeed * speedMultiplier);
			break;
		case PublicVariableType::Vec3:
			ImGui::DragFloat3(editorVariable->name, (float*)editorVariable->data, editorVariable->editor_sliderSpeed * speedMultiplier);
			break;
		case PublicVariableType::String:
			ImGui::InputText(editorVariable->name, (char*)editorVariable->data, 256);
			break;
		case PublicVariableType::Color:
			ImGui::ColorPicker3(editorVariable->name, (float*)editorVariable->data);
			break;
		}
	}

}