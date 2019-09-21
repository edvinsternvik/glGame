#include "GuiWindow.h"
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Scene.h"

namespace glGame {

	ViewportWindow::ViewportWindow(unsigned int texture, float aspectRatio) : m_texture(texture), m_aspectRatio(aspectRatio) {	
		
	}

	void ViewportWindow::renderWindow() {
		float width = ImGui::GetContentRegionAvail().x;
		float height = width / m_aspectRatio;
		ImGui::Image((void*)(intptr_t)m_texture, ImVec2(width, height));
	}

	SceneWindow::SceneWindow(std::unique_ptr<Scene>* scene) : m_scene(scene) {
	}

	void SceneWindow::renderWindow() {
		if (ImGui::Button("Create GameObject", ImVec2(0, 0))) {
			(*m_scene)->createGameObject("New GameObject");
		}
		ImGui::Separator();
		//ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

		int gameObjects = (*m_scene)->getGameObjectCount();
		for (int i = 0; i < gameObjects; ++i) {
			if (ImGui::Button((*m_scene)->getGameObject(i)->name.c_str(), ImVec2(0, 0))) {
				(*m_scene)->selectGameObject(i);
			}
			//ImGui::TreeNodeEx((*m_scene)->getGameObject(i)->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
		}
	}

	PropertiesWindow::PropertiesWindow(std::unique_ptr<Scene>* scene) : m_scene(scene) {
	}

	void PropertiesWindow::renderWindow() {
		if ((*m_scene)->getSelectedGameObject()) {
			ImGui::Text("hello");
		}
	}

}