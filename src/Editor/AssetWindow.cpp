#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Resources/AssetManager.h"
#include <iostream>

namespace glGame {

    AssetWindow::AssetWindow(AssetManager* assetManager) : m_assetManager(assetManager) {

    }

    AssetWindow::AssetWindow() : m_assetManager(nullptr) {

    }

    void AssetWindow::renderWindow() {
        if(m_assetManager == nullptr) return;

        ImGuiStyle& style = ImGui::GetStyle();
        float window_visible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        int number = 0, totalAssetCount = m_assetManager->assetCount();
        for(auto it = m_assetManager->getAssetsBegin(); it != m_assetManager->getAssetsEnd(); ++it, ++number) {
            ImVec2 buttonSize = ImGui::CalcTextSize(it->first.c_str());
            buttonSize = ImVec2(buttonSize.x + 25, 50);

            float nextSize = ImGui::GetItemRectMax().x + buttonSize.x + style.ItemSpacing.x;
            if(number > 0 && nextSize < window_visible) ImGui::SameLine();

            ImGui::PushID(number);
            ImGui::Button(it->first.c_str(), buttonSize);
            ImGui::PopID();

            if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                ImGui::SetDragDropPayload(it->second.assetTypeString.c_str(), &it->first, sizeof(it->first));
                ImGui::Text(it->first.c_str());
                ImGui::EndDragDropSource();
            }
        }
    }

}