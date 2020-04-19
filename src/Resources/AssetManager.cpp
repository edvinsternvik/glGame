#include "AssetManager.h"

#include "Model.h"

#include <filesystem>
#include <iostream>

namespace glGame {

    AssetManager::AssetManager() {

    }

    AssetManager::~AssetManager() {

    }

    Asset* AssetManager::getAsset(const char* name, AssetType type) {
        auto search = m_assets.find(name);
        if(search == m_assets.end()) {
            return nullptr;
        }

        return search->second.get();
    }

    void AssetManager::updateAssets() {
        m_assets.clear();

        std::filesystem::path assetsPath("./Assets");
        if(std::filesystem::exists(assetsPath) && std::filesystem::is_directory(assetsPath)) {
            for(const auto& entry : std::filesystem::recursive_directory_iterator(assetsPath)) {
                if(std::filesystem::is_regular_file(entry.status())) {
                    std::string extension = entry.path().extension(), filename = entry.path().filename(), pathStr = entry.path().string();
                    
                    if(extension == ".obj")
                        insertAsset(filename, std::make_unique<Model>(pathStr.c_str()));
                    // else if(extension == ".script")
                        // insertAsset(filename, std::make_unique<ScriptAsset>(pathStr.c_str()))
                    // else if(extension == ".png" || extension == ".jpg")
                        // insertAsset(filename, std::make_unique<ImageAsset>(pathStr.c_str()));
                }
            }
        }
        
    }

    void AssetManager::insertAsset(const std::string& name, std::unique_ptr<Asset> asset) {
        if(m_assets.find(name.c_str()) == m_assets.end())
            m_assets.insert(std::pair<std::string, std::unique_ptr<Asset>>((std::string)name, std::move(asset)));
    }

}