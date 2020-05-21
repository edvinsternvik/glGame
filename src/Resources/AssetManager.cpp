#include "AssetManager.h"

#include "Model.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace glGame {

    AssetManager* AssetManager::s_instance = nullptr;

    AssetManager::AssetManager() {
        s_instance = this;

        m_highestId = 0;
        initFromFile();

        updateAssets();
    }

    AssetManager::~AssetManager() {
        // #ifdef GL_GAME_DEBUG
        saveToFile();
        // #endif
    }

    Asset* AssetManager::getAsset(unsigned int id, AssetType type) {
        auto search = m_assets.find(id);
        if(search == m_assets.end()) {
            return nullptr;
        }

        return search->second.get();
    }

    void AssetManager::updateAssets() {
        std::filesystem::path assetsPath("./Assets");
        if(std::filesystem::exists(assetsPath) && std::filesystem::is_directory(assetsPath)) {
            for(const auto& entry : std::filesystem::recursive_directory_iterator(assetsPath)) {
                if(std::filesystem::is_regular_file(entry.status())) {
                    std::string extension = entry.path().extension().u8string(), filename = entry.path().filename().u8string(), pathStr = entry.path().u8string();
                    
                    bool exist = false;
                    for(auto& it : m_assets) {
                        if(pathStr == it.second->name) {
                            exist = true;
                            break;
                        }
                    }

                    if(!exist) addAsset(pathStr, extension);
                }
            }
        }
        
    }

    void AssetManager::initFromFile() {
        const char* filename = "assetconfig";

        std::ifstream file(filename);
        if(!file.is_open()) {
            return;
        }

        std::string line;
        while(std::getline(file, line)) {
            auto delimPos = line.find_first_of(':');
            std::string idStr = line.substr(0, delimPos);
            std::string path = line.substr(delimPos + 1);

            unsigned int id = (unsigned int)std::stoi(idStr);
            if(std::filesystem::exists(path)) {
                delimPos = path.find_last_of('.');
                std::string extension = path.substr(delimPos);

                int tempHighestID = m_highestId;
                m_highestId = id - 1; 
                addAsset(path, extension);
                if(tempHighestID > m_highestId) m_highestId = tempHighestID;
            }
            else {
                if(id > m_highestId) m_highestId = id;
                m_deletedAssets.push_back(std::pair<unsigned int, std::string>(id, path));
            }
        }
    }

    void AssetManager::saveToFile() {
        const char* filename = "assetconfig";
        
        std::fstream file(filename);
        if(!file.is_open()) {
            std::cout << "Could not open file " << filename << std::endl;
            return;
        }

        for(auto& assetPair : m_assets) {
            file << assetPair.first << ":" << assetPair.second->name << "\n";
        }

        for(auto& assetPair : m_deletedAssets) {
            file << assetPair.first << ":" << assetPair.second << "\n";
        }

        file.close();
    }

    void AssetManager::addAsset(std::string& filepath, std::string& extension) {
        if(extension == ".obj")
            insertAsset(std::make_unique<Model>(filepath.c_str()));
        // else if(extension == ".script")
            // insertAsset(filename, std::make_unique<ScriptAsset>(pathStr.c_str()))
        // else if(extension == ".png" || extension == ".jpg")
            // insertAsset(filename, std::make_unique<ImageAsset>(pathStr.c_str()));
    }

    void AssetManager::insertAsset(std::unique_ptr<Asset> asset) {
        m_assets.insert(std::pair<unsigned int, std::unique_ptr<Asset>>(++m_highestId, std::move(asset)));
    }

}