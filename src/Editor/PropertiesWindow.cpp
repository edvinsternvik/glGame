#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Resources/Scene.h"

namespace glGame {

	void InputText(const char* name, std::string& str, int maxSize);

	PropertiesWindow::PropertiesWindow(Scene* scene) : m_scene(scene) {
	}

	void PropertiesWindow::renderWindow() {
		GameObject* selectedObj = m_scene->getSelectedGameObject();

		if (!selectedObj) {
			return;
		}

		ImGui::Spacing();

		InputText("###NAME", selectedObj->name, 64);

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
		case PublicVariableType::String: {
			InputText(editorVariable->name, *(std::string*)editorVariable->data, 64);
			break;
		}
		case PublicVariableType::Color:
			ImGui::ColorPicker3(editorVariable->name, (float*)editorVariable->data);
			break;
		}
	}

	void InputText(const char* name, std::string& str, int maxSize) {
		int editorVariableStrLen = str.size();
		std::vector<char> charBuffer(maxSize);
		for(int i = 0; i < editorVariableStrLen; ++i) charBuffer[i] = str[i];
		if(ImGui::InputText(name, charBuffer.data(), 64)) {
			if(charBuffer[0] != 0 && strcmp(str.c_str(), charBuffer.data()) != 0) {
				str.assign(charBuffer.data());
			}
		}
	}

}