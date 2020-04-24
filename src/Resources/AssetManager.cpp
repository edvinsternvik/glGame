#include "AssetManager.h"

#include "Model.h"

#include <filesystem>
#include <iostream>

namespace glGame {

    AssetManager* AssetManager::s_instance = nullptr;

    AssetManager::AssetManager() {
        initFromFile();
        s_instance = this;

        m_highestId = 0;
        for(const auto& asset : m_assets) {
            if(asset.first > m_highestId) m_highestId = asset.first;
        }

        updateAssets();
    }

    AssetManager::~AssetManager() {

    }

    Asset* AssetManager::getAsset(unsigned int id, AssetType type) {
        auto search = m_assets.find(id);
        if(search == m_assets.end()) {
            return nullptr;
        }

        return search->second.get();
    }

    void AssetManager::updateAssets() {
        m_assets.clear(); // Temporary

        std::filesystem::path assetsPath("./Assets");
        if(std::filesystem::exists(assetsPath) && std::filesystem::is_directory(assetsPath)) {
            for(const auto& entry : std::filesystem::recursive_directory_iterator(assetsPath)) {
                if(std::filesystem::is_regular_file(entry.status())) {
                    std::string extension = entry.path().extension(), filename = entry.path().filename(), pathStr = entry.path().string();
                    
                    if(extension == ".obj")
                        insertAsset(std::make_unique<Model>(pathStr.c_str()));
                    // else if(extension == ".script")
                        // insertAsset(filename, std::make_unique<ScriptAsset>(pathStr.c_str()))
                    // else if(extension == ".png" || extension == ".jpg")
                        // insertAsset(filename, std::make_unique<ImageAsset>(pathStr.c_str()));
                }
            }
        }
        
    }

    void AssetManager::initFromFile() {

    }

    void AssetManager::insertAsset(std::unique_ptr<Asset> asset) {
        m_assets.insert(std::pair<unsigned int, std::unique_ptr<Asset>>(++m_highestId, std::move(asset)));
    }

}