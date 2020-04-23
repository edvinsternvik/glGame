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
        float currentLineX = 0.0;
        for(auto it = m_assetManager->getAssetsBegin(); it != m_assetManager->getAssetsEnd(); ++it, ++number) {
            ImVec2 buttonSize = ImGui::CalcTextSize(it->second->name.c_str());
            buttonSize = ImVec2(buttonSize.x + 25, 50);

            currentLineX += buttonSize.x + style.ItemSpacing.x;
            if(currentLineX < window_visible) ImGui::SameLine();
            else currentLineX = 0.0;

            ImGui::PushID(number);
            ImGui::Button(it->second->name.c_str(), buttonSize);
            ImGui::PopID();

            unsigned int data = it->first;
            if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                ImGui::SetDragDropPayload(it->second->getAssetTypeString(), &data, sizeof(unsigned int));
                ImGui::Text(it->second->getAssetTypeString());
                ImGui::EndDragDropSource();
            }
        }
    }

}