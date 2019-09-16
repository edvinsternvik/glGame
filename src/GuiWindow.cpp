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

	SceneWindow::SceneWindow(std::shared_ptr<Scene> const& scene) : m_scene(scene) {
	}

	void SceneWindow::renderWindow() {
		if (ImGui::Button("Create GameObject", ImVec2(0, 0))) {
			m_scene->createGameObject("New GameObject");
		}
		ImGui::Separator();

		int gameObjects = m_scene->getGameObjectCount();
		for (int i = 0; i < gameObjects; ++i) {
			ImGui::Text(m_scene->getGameObject(i)->name.c_str());
		}
	}

}