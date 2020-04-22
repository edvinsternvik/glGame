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

        for(auto it = m_assetManager->getAssetsBegin(); it != m_assetManager->getAssetsEnd(); ++it) {
            ImGui::Text(it->second->name.c_str());
        }
    }

}