#include "AssetManager.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace glGame {

    AssetManager* AssetManager::s_instance = nullptr;

    AssetManager::AssetManager() {
        s_instance = this;

        updateAssets();
    }

    void AssetManager::updateAssets() {
        std::filesystem::path assetsPath("./Assets");
        if(std::filesystem::exists(assetsPath) && std::filesystem::is_directory(assetsPath)) {
            for(const auto& entry : std::filesystem::recursive_directory_iterator(assetsPath)) {
                if(std::filesystem::is_regular_file(entry.status())) {
                    std::string extension = entry.path().extension().u8string(), filename = entry.path().filename().u8string(), pathStr = entry.path().u8string();
                    
                    auto search = m_assets.find(pathStr);
                    if(search == m_assets.end()) {
                        makeEmptyAssetData(pathStr, extension);
                    }
                }
            }
        }
        
    }

    void AssetManager::makeEmptyAssetData(const std::string& assetName, const std::string& extension) {
        AssetData assetData;
        if(extension == ".obj")
            assetData = AssetData(AssetType::Model, "Model");
        else if(extension == ".as")
            assetData = AssetData(AssetType::Script, "Script");
        else if(extension == ".png" || extension == ".jpg" || extension == ".jpeg")
            assetData = AssetData(AssetType::Texture, "Texture");
        else if(extension == ".glsl" || extension == ".shader")
            assetData = AssetData(AssetType::Shader, "Shader");
        else {
            return;
        }

        m_assets.insert(std::pair<std::string, AssetData>(assetName, assetData));
    }
}