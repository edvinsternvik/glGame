#include "GuiWindow.h"
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Resources/Scene.h"
#include "Editor.h"
#include "../Input.h"

namespace glGame {

	ViewportWindow::ViewportWindow(unsigned int texture, float aspectRatio) : m_texture(texture), m_aspectRatio(aspectRatio) {	
		
	}

	void ViewportWindow::renderWindow() {
		if(ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow)) {
			if(!m_focused) {
				GameFocused e;
				if(m_eventFunction) (*m_eventFunction)(e);
				m_focused = true;
			}
		}
		else {
			if(m_focused) {
				GameUnfocused e;
				if(m_eventFunction) (*m_eventFunction)(e);
				m_focused = false;
			}
		}

		viewportX = ImGui::GetCursorScreenPos().x;
		viewportY = ImGui::GetCursorScreenPos().y;
		viewportWidth = ImGui::GetContentRegionAvail().x;
		viewportHeight = viewportWidth / m_aspectRatio;
		ImGui::Image((void*)(intptr_t)m_texture, ImVec2(viewportWidth, viewportHeight), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));

		Input::setViewportOffset(Vector2(viewportX, viewportY));
	}

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