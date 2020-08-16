#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Resources/Scene.h"
#include "../Components/ComponentList.h"
#include "../Resources/AssetManager.h"
#include "../Rendering/Shader.h"

#include "Editor.h"

namespace glGame {

	void InputText(const char* name, std::string& str, int maxSize);
	template<typename T> void registerChangePublicVariableAction(T* data, ActionManager* actionManager);
	template<class T> void drawAssetVariableGui(const PublicVariable* editorVariable, const char* payloadTarget);

	PropertiesWindow::PropertiesWindow(Scene* scene) : m_scene(scene) {
	}

	void PropertiesWindow::setScene(Scene* scene) {
		m_scene = scene;
	}

	PropertiesWindow::PropertiesWindow() : m_scene(nullptr) {}

	void PropertiesWindow::renderWindow() {
		if(!m_editor->getSelectedItem<GameObject>().expired()) {
			drawGameObjectPropertiesWindow(m_editor->getSelectedItem<GameObject>().lock());
		}
		else if(!m_editor->getSelectedItem<assetInternal::AssetT>().expired()) {
			drawAssetPropertiesWindow(m_editor->getSelectedItem<assetInternal::AssetT>().lock());
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
		case toInt(PublicVariableType::Boolean):
			ImGui::Checkbox(editorVariable->name.c_str(), std::get<bool*>(editorVariable->data));
			registerChangePublicVariableAction<bool>(std::get<bool*>(editorVariable->data), &m_editor->actionManager);
			break;
		case toInt(PublicVariableType::Vec2):
			ImGui::DragFloat2(editorVariable->name.c_str(), (float*)std::get<Vector2*>(editorVariable->data), editorVariable->editor_sliderSpeed * speedMultiplier);
			registerChangePublicVariableAction<Vector2>(std::get<Vector2*>(editorVariable->data), &m_editor->actionManager);
			break;
		case toInt(PublicVariableType::Vec2i):
			ImGui::DragInt2(editorVariable->name.c_str(), (int*)std::get<Vector2i*>(editorVariable->data), editorVariable->editor_sliderSpeed * speedMultiplier);
			registerChangePublicVariableAction<Vector2i>(std::get<Vector2i*>(editorVariable->data), &m_editor->actionManager);
			break;
		case toInt(PublicVariableType::Vec3):
			ImGui::DragFloat3(editorVariable->name.c_str(), (float*)std::get<Vector3*>(editorVariable->data), editorVariable->editor_sliderSpeed * speedMultiplier);
			registerChangePublicVariableAction<Vector3>(std::get<Vector3*>(editorVariable->data), &m_editor->actionManager);
			break;
		case toInt(PublicVariableType::Vec3i):
			ImGui::DragInt3(editorVariable->name.c_str(), (int*)std::get<Vector3i*>(editorVariable->data), editorVariable->editor_sliderSpeed * speedMultiplier);
			registerChangePublicVariableAction<Vector3i>(std::get<Vector3i*>(editorVariable->data), &m_editor->actionManager);
			break;
		// case PublicVariableType::Color:
		// 	ImGui::ColorPicker3(editorVariable->name.c_str(), (float*)editorVariable->data);
		// 	break;
		case toInt(PublicVariableType::PublicVariableEnum): {
			PublicVariableEnum* pVarEnum = std::get<PublicVariableEnum*>(editorVariable->data);
			if(ImGui::Combo(editorVariable->name.c_str(), &pVarEnum->selection, pVarEnum->options.data(), pVarEnum->options.size())) {
				m_editor->actionManager.endChangePublicVariableAction<PublicVariableEnum>(pVarEnum, *pVarEnum);
			}
			if(ImGui::IsItemActivated()) m_editor->actionManager.beginChangePublicVariableAction<PublicVariableEnum>(pVarEnum, *pVarEnum);
			break;
		}
		case toInt(PublicVariableType::Model): drawAssetVariableGui<Model>(editorVariable, "Model"); break;
		case toInt(PublicVariableType::Script): drawAssetVariableGui<Script>(editorVariable, "Script"); break;
		case toInt(PublicVariableType::Texture): drawAssetVariableGui<Texture>(editorVariable, "Texture"); break;
		case toInt(PublicVariableType::Shader): drawAssetVariableGui<Shader>(editorVariable, "Shader"); break;
		case toInt(PublicVariableType::Material): drawAssetVariableGui<Material>(editorVariable, "Material"); break;
		}
	}

	void PropertiesWindow::drawGameObjectPropertiesWindow(std::shared_ptr<GameObject> selectedObj) {
		if(m_scene == nullptr) return;

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
					std::shared_ptr<Component> newComponent = selectedObj->addComponent(componentStr);
					m_editor->actionManager.addCreateComponentAction(newComponent);
					ImGui::CloseCurrentPopup();
				}
			}

			for(const char* component : addComponentDisabled) {
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25);
				ImGui::Button(component);
				ImGui::PopStyleVar();
			}

			ImGui::EndPopup();
		}
	}

	void PropertiesWindow::drawAssetPropertiesWindow(std::shared_ptr<assetInternal::AssetT> selectedAsset) {
		if (!selectedAsset.get()) {
			return;
		}

		ImGui::Spacing();
		ImGui::Text(selectedAsset->filepath.c_str());

		ImGuiStyle& style = ImGui::GetStyle();
		float window_visible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		float titleSize = ImGui::GetItemRectMax().x;
		float nextSize = titleSize + ImGui::CalcTextSize("Refresh").x + style.ItemSpacing.x;
		if(nextSize < window_visible) ImGui::SameLine();
		if(ImGui::SmallButton("Refresh##ID")) {
			selectedAsset->reload();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		for(auto& pVar : selectedAsset->m_publicVariables) {
			drawComponentVariableGui(&pVar);

			ImGui::Spacing();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing;

		if(ImGui::Button("Save asset")) {
			AssetManager::Get().saveAsset(selectedAsset);
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

	template<class T>
	void PropertiesWindow::drawAssetVariableGui(const PublicVariable* editorVariable, const char* payloadTarget) {
		Asset<T> asset = *std::get<Asset<T>*>(editorVariable->data);
		std::string assetName = asset.get() != nullptr ? asset.getPath() : "";
		ImGui::Text((editorVariable->name + ": " + assetName).c_str());
		if(ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text(payloadTarget);
			ImGui::EndTooltip();
		}
		if(ImGui::BeginDragDropTarget()) {
			if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadTarget)) {
				std::string payloadData = *(std::string*)payload->Data;
				m_editor->actionManager.beginChangePublicVariableAction<Asset<T>>(std::get<Asset<T>*>(editorVariable->data), *std::get<Asset<T>*>(editorVariable->data));
				*std::get<Asset<T>*>(editorVariable->data) = AssetManager::Get().getAsset<T>(payloadData);
				m_editor->actionManager.endChangePublicVariableAction<Asset<T>>(std::get<Asset<T>*>(editorVariable->data), *std::get<Asset<T>*>(editorVariable->data));
			}

			ImGui::EndDragDropTarget();
		}
	}

	template<typename T>
	void registerChangePublicVariableAction(T* data, ActionManager* actionManager) {
		if(ImGui::IsItemActivated()) actionManager->beginChangePublicVariableAction<T>(data, *data);
		if(ImGui::IsItemDeactivatedAfterEdit()) actionManager->endChangePublicVariableAction<T>(data, *data);
	}

}