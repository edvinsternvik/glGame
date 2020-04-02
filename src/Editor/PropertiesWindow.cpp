#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Resources/Scene.h"
#include "../Components/Script.h"
#include "../Components/ComponentList.h"

namespace glGame {

	void InputText(const char* name, std::string& str, int maxSize);

	PropertiesWindow::PropertiesWindow(Scene* scene) : m_scene(scene) {
	}

	void PropertiesWindow::renderWindow() {
		static std::string s_addScriptFilenameBuffer = "";
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

		int componentVariableIndex = 0;
		for(int i = 0; i < selectedObj->getComponentSize(); ++i) {
			const Component* selectedComponent = selectedObj->getComponent(i);
			ImGui::Text(selectedComponent->getName().c_str());
			if(selectedComponent->getName() != "Transform") {
				ImGui::SameLine();
				ImGui::PushID(i);
				if(ImGui::SmallButton("X##ID")) {
					selectedObj->removeComponent(i);
					ImGui::PopID();
					continue;
				}
				ImGui::PopID();
			}

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

		ImGui::SameLine(50);
		if(ImGui::Button("Add Component", ImVec2(-50, 25))) {
			ImGui::OpenPopup("AddComponentPopup");
		}

		if(ImGui::BeginPopup("AddComponentPopup")) {
			std::vector<const char*> addComponentEnabled, addComponentDisabled;
			for(const char* component : componentList::s_components) {
				bool disabled = false;
				if(component == "Transform") disabled = true;
				if(component == "MeshRenderer" && selectedObj->meshRenderer != nullptr) disabled = true;

				if(disabled) addComponentDisabled.push_back(component);
				else addComponentEnabled.push_back(component);
			}

			for(const char* component : addComponentEnabled) {
				if(ImGui::Button(component)) {
					std::string componentStr = std::string(component);
					if(componentStr == "Script") {
						ImGui::OpenPopup("AddScriptPopup");
						s_addScriptFilenameBuffer = "";
					}
					else {
						selectedObj->addComponent(componentStr);
					}
				}
			}

			for(const char* component : addComponentDisabled) {
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25);
				ImGui::Button(component);
				ImGui::PopStyleVar();
			}
			
			if(ImGui::BeginPopup("AddScriptPopup")) {
				InputText("filename", s_addScriptFilenameBuffer, 64);

				if(ImGui::Button("Add Script")) {
					Script* s = selectedObj->addComponent<Script>();
					std::cout << s_addScriptFilenameBuffer << std::endl;
					s->changeScriptfile(s_addScriptFilenameBuffer);
				}

				ImGui::EndPopup();
			}

			ImGui::EndPopup();
		}
	}

	void PropertiesWindow::drawComponentVariableGui(const PublicVariable* editorVariable) {
		float speedMultiplier = 0.01f;
		switch(editorVariable->variableType) {

		case PublicVariableType::Int:
			ImGui::DragInt(editorVariable->name.c_str(), (int*)editorVariable->data, editorVariable->editor_sliderSpeed * speedMultiplier);
			break;
		case PublicVariableType::Float:
			ImGui::DragFloat(editorVariable->name.c_str(), (float*)editorVariable->data, editorVariable->editor_sliderSpeed * speedMultiplier);
			break;
		case PublicVariableType::Double:
			ImGui::InputDouble(editorVariable->name.c_str(), (double*)editorVariable->data);
			break;
		case PublicVariableType::Vec2:
			ImGui::DragFloat2(editorVariable->name.c_str(), (float*)editorVariable->data, editorVariable->editor_sliderSpeed * speedMultiplier);
			break;
		case PublicVariableType::Vec3:
			ImGui::DragFloat3(editorVariable->name.c_str(), (float*)editorVariable->data, editorVariable->editor_sliderSpeed * speedMultiplier);
			break;
		case PublicVariableType::String: {
			InputText(editorVariable->name.c_str(), *(std::string*)editorVariable->data, 64);
			break;
		}
		case PublicVariableType::Color:
			ImGui::ColorPicker3(editorVariable->name.c_str(), (float*)editorVariable->data);
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