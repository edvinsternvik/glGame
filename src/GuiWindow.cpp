#include "GuiWindow.h"
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace glGame {

	ViewportWindow::ViewportWindow(unsigned int texture, float aspectRatio) : m_texture(texture), m_aspectRatio(aspectRatio) {
		
	}

	void ViewportWindow::renderWindow() {
		float width = ImGui::GetContentRegionAvail().x;
		float height = width / m_aspectRatio;
		ImGui::Image((void*)(intptr_t)m_texture, ImVec2(width, height));
	}

}