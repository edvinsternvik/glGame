#include "GuiWindow.h"

#include "../Application.h"
#include "../Rendering/Texture.h"
#include "Editor.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace glGame {

    ViewportWindow::ViewportWindow(Vector2i viewportSize) : m_viewportSize(viewportSize) {
		windowFlags = ImGuiWindowFlags_MenuBar;
	}

    ViewportWindow::~ViewportWindow() {

    }

	void ViewportWindow::setTexture(std::shared_ptr<Texture> texture) {
		m_viewportTexture = texture;
	}

	void ViewportWindow::renderWindow() {
		if (ImGui::BeginMenuBar()) {
			auto showTransformTypeSelectable = [this](const char* name, TransformType t){
				if(ImGui::Selectable(name, m_editor->transformType == t, 0, ImVec2(45, 10))) {
					m_editor->transformType = t;
				}
			};

			showTransformTypeSelectable("Move", TransformType::Move);
			showTransformTypeSelectable("Rotate", TransformType::Rotate);
			showTransformTypeSelectable("Scale", TransformType::Scale);

			ImGui::EndMenuBar();
		}
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

		float aspectRatio = (float)m_viewportSize.x / (float)m_viewportSize.y;
		viewportX = ImGui::GetCursorScreenPos().x;
		viewportY = ImGui::GetCursorScreenPos().y;
		viewportWidth = ImGui::GetContentRegionAvail().x;
		viewportHeight = viewportWidth / aspectRatio;
        
		Input::setViewportOffset(Vector2(viewportX, viewportY));
		Input::setViewportSize(Vector2(viewportWidth, viewportHeight));

		if(m_viewportTexture.get()) {
			ImGui::Image((void*)(intptr_t)m_viewportTexture->getTextureId(), ImVec2(viewportWidth, viewportHeight), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
		}
	}

}