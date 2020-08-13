#include "GuiWindow.h"
#include "../Resources/Scene.h"
#include "Editor.h"
#include "../Input.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

namespace glGame {

	SceneWindow::SceneWindow(Scene* scene) : m_scene(scene) {
	}

	SceneWindow::SceneWindow() : m_scene(nullptr) {
	}

	void SceneWindow::renderWindow() {
		if(m_scene == nullptr) return;

		if (ImGui::Button("Create GameObject", ImVec2(0, 0))) {
			std::shared_ptr<GameObject> newGameObject = m_scene->createGameObject("New GameObject");
			m_editor->actionManager.addCreateGameObjectAction(newGameObject, m_scene);
		}
		ImGui::Separator();

		GameObject* current = nullptr;
		if(!m_editor->getSelectedItem<GameObject>().expired()) current = m_editor->getSelectedItem<GameObject>().lock().get();
		
		int gameObjects = m_scene->getGameObjectCount();
		for (int i = 0; i < gameObjects; ++i) {
			bool selected = (current == m_scene->getGameObject(i).lock().get());
			ImGui::PushID(i);
			if (ImGui::Selectable(m_scene->getGameObject(i).lock()->name.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns)) {
				std::weak_ptr<GameObject> gameObjectWeak = m_scene->getGameObjectShared(i);
				m_editor->selectItem(gameObjectWeak);
			}
			ImGui::PopID();
		}
	}

}