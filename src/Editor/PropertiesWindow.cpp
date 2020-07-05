#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Resources/Scene.h"
#include "../Components/ComponentList.h"
#include "../Resources/AssetManager.h"

#include "Editor.h"

namespace glGame {

	void InputText(const char* name, std::string& str, int maxSize);

	template<typename T>
	void registerChangePublicVariableAction(T* data, ActionManager* actionManager);

	PropertiesWindow::PropertiesWindow(Scene* scene) : m_scene(scene) {
	}

	void PropertiesWindow::setScene(Scene* scene) {
		m_scene = scene;
	}

	PropertiesWindow::PropertiesWindow() : m_scene(nullptr) {}

	void PropertiesWindow::renderWindow() {
		if(m_scene == nullptr) return;

		static std::string s_stringBuffer = "";
		std::shared_ptr<GameObject> selectedObj = m_scene->getSelectedGameObject();

		if (!selectedObj.get()) {
			return;
		}

		ImGui::Spacing();

		InputText("###NAME", selectedObj->name, 64);
		registerChangePublicVariableAction<std::string>(&selectedObj->name, &m_editor->actionManager);
		ImGui::SameLine();
		if(ImGui::Button("Delete")) {
			m_scene->deleteGameObject(selectedObj.get());
			m_editor->actionManager.addDeleteGameObjectAction(selectedObj, m_scene);
			return;
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

		int componentVariableIndex = 0;
		for(int i = 0; i < selectedObj->getComponentSize(); ++i) {
			const std::shared_ptr<Component> selectedComponent = selectedObj->getComponent(i);
			ImGui::Text(selectedComponent->getName().c_str());
			if(selectedComponent->getName() != "Transform") {
				ImGui::SameLine();
				ImGui::PushID(i);
				if(ImGui::SmallButton("X##ID")) {
					selectedObj->removeComponent(i);
					m_editor->actionManager.addDeleteComponentAction(selectedComponent);
					ImGui::PopID();
					continue;
				}

				ImGui::SameLine();
				if(ImGui::SmallButton("Refresh##ID")) {
					selectedObj->addComponentToInitQueue(i);
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

				if(disabled) addComponentDisabled.push_back(component);
				else addComponentEnabled.push_back(component);
			}

			for(const char* component : addComponentEnabled) {
				if(ImGui::Button(component)) {
					std::string componentStr = std::string(component);
					s_stringBuffer = "";
					if(componentStr == "Script") {
						ImGui::OpenPopup("AddScriptPopup");
					}
					else {
						std::shared_ptr<Component> newComponent = selectedObj->addComponent(componentStr);
						m_editor->actionManager.addCreateComponentAction(newComponent);
						ImGui::CloseCurrentPopup();
					}
				}
			}

			for(const char* component : addComponentDisabled) {
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25);
				ImGui::Button(component);
				ImGui::PopStyleVar();
			}
			
			// if(ImGui::BeginPopup("AddScriptPopup")) {
			// 	InputText("filename", s_stringBuffer, 64);
			// 	if(ImGui::Button("Add Script")) {
			// 		std::shared_ptr<Script> s = selectedObj->addComponent<Script>();
			// 		s->changeScriptfile(s_stringBuffer);
			// 		m_editor->actionManager.addCreateComponentAction(s);
			// 		ImGui::CloseCurrentPopup();
			// 	}
			// 	ImGui::EndPopup();
			// }

			ImGui::EndPopup();
		}
	}

	void PropertiesWindow::drawComponentVariableGui(const PublicVariable* editorVariable) {
		float speedMultiplier = 0.01f;
		switch(editorVariable->data.index()) {
		case toInt(PublicVariableType::Int):
			ImGui::DragInt(editorVariable->name.c_str(), std::get<int*>(editorVariable->data), editorVariable->editor_sliderSpeed * speedMultiplier);
			registerChangePublicVariableAction<int>(std::get<int*>(editorVariable->data), &m_editor->actionManager);
			break;
		case toInt(PublicVariableType::Float):
			ImGui::DragFloat(editorVariable->name.c_str(), std::get<float*>(editorVariable->data), editorVariable->editor_sliderSpeed * speedMultiplier);
			registerChangePublicVariableAction<float>(std::get<float*>(editorVariable->data), &m_editor->actionManager);
			break;
		case toInt(PublicVariableType::Double):
			ImGui::InputDouble(editorVariable->name.c_str(), std::get<double*>(editorVariable->data));
			registerChangePublicVariableAction<double>(std::get<double*>(editorVariable->data), &m_editor->actionManager);
			break;
		case toInt(PublicVariableType::String): {
			InputText(editorVariable->name.c_str(), *std::get<std::string*>(editorVariable->data), 64);
			registerChangePublicVariableAction<std::string>(std::get<std::string*>(editorVariable->data), &m_editor->actionManager);
			break;
		}
		case toInt(PublicVariableType::Vec2):
			ImGui::DragFloat2(editorVariable->name.c_str(), (float*)std::get<Vector2*>(editorVariable->data), editorVariable->editor_sliderSpeed * speedMultiplier);
			registerChangePublicVariableAction<Vector2>(std::get<Vector2*>(editorVariable->data), &m_editor->actionManager);
			break;
		case toInt(PublicVariableType::Vec3):
			ImGui::DragFloat3(editorVariable->name.c_str(), (float*)std::get<Vector3*>(editorVariable->data), editorVariable->editor_sliderSpeed * speedMultiplier);
			registerChangePublicVariableAction<Vector3>(std::get<Vector3*>(editorVariable->data), &m_editor->actionManager);
			break;
		// case PublicVariableType::Color:
		// 	ImGui::ColorPicker3(editorVariable->name.c_str(), (float*)editorVariable->data);
		// 	break;
		case toInt(PublicVariableType::Model): {
			ModelAsset model = *std::get<ModelAsset*>(editorVariable->data);
			std::string modelName = model.get() != nullptr ? model->name : "";
			ImGui::Text(("Model: " + modelName).c_str());
			if(ImGui::BeginDragDropTarget()) {
				if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Model")) {
					std::string payloadData = *(std::string*)payload->Data;
					m_editor->actionManager.beginChangePublicVariableAction<ModelAsset>(std::get<ModelAsset*>(editorVariable->data), *std::get<ModelAsset*>(editorVariable->data));
					*std::get<ModelAsset*>(editorVariable->data) = AssetManager::Get().getAsset<Model>(payloadData);
					m_editor->actionManager.endChangePublicVariableAction<ModelAsset>(std::get<ModelAsset*>(editorVariable->data), *std::get<ModelAsset*>(editorVariable->data));
				}

				ImGui::EndDragDropTarget();
			}
		}
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

	template<typename T>
	void registerChangePublicVariableAction(T* data, ActionManager* actionManager) {
		if(ImGui::IsItemActivated()) actionManager->beginChangePublicVariableAction<T>(data, *data);
		if(ImGui::IsItemDeactivatedAfterEdit()) actionManager->endChangePublicVariableAction<T>(data, *data);
	}

}