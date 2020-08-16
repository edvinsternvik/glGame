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

    void AssetManager::saveAsset(std::shared_ptr<assetInternal::AssetT> asset) {
        if(asset->m_publicVariables.size() < 1) return;

        std::string assetdataPathString = getAssetDataPath(asset, true);

        std::fstream assetdataFileStream;
        assetdataFileStream.open(assetdataPathString, std::ios::out);
        if(!assetdataFileStream.is_open()) {
            std::cout << "Could not open: " << assetdataPathString << std::endl;
            return;
        }
        
        for(PublicVariable& pvar : asset->m_publicVariables) {
            assetdataFileStream << pvar.name << " " << pvar.getPublicVariableString() << "\n";
        }

        assetdataFileStream.close();
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

    bool AssetManager::initializePublicVariables(std::shared_ptr<assetInternal::AssetT> asset) {
        std::string assetdataPathString = getAssetDataPath(asset, false);
        
        std::fstream assetdataFileStream;
        assetdataFileStream.open(assetdataPathString, std::ios::in);
        if(!assetdataFileStream.is_open()) {
            return false;
        }

        bool changedVariable = false;

        std::string line;
		while(std::getline(assetdataFileStream, line)) {
            std::stringstream linebuffer;
			linebuffer << line;

            std::vector<std::string> strings;
			std::string word;
			while(std::getline(linebuffer, word, ' ')) {
				strings.push_back(word);
			}

            if(strings.size() < 2) {
                std::cout << "Error formating in file: " << assetdataPathString << " on line: " << line << std::endl;
                break;
            }

            std::string pVarName; // Combine all the strings except the last one
            for(int i = 0; i < strings.size() - 1; ++i) pVarName += strings[i];

            PublicVariable* pVar = asset->getPublicVariable(pVarName);
            if(pVar) {
                pVar->setData(strings[strings.size() - 1]);
                changedVariable = true;
            }
        }

        assetdataFileStream.close();
        return changedVariable;
    }

    std::string AssetManager::getAssetDataPath(std::shared_ptr<assetInternal::AssetT>& asset, bool createPath) {
        if(!asset->hasData()) return asset->filepath;
        
        std::filesystem::path assetPath(asset->filepath);
        if(!std::filesystem::exists(assetPath) || !std::filesystem::is_regular_file(assetPath)) {
            return "";
        }

        std::filesystem::path assetdataFolderPath = "./Assets/.assetdata";
        if(createPath && (!std::filesystem::exists(assetdataFolderPath) || !std::filesystem::is_directory(assetdataFolderPath))) {
            std::filesystem::create_directory(assetdataFolderPath);
        }
        
        std::filesystem::path assetdataPath = assetdataFolderPath / std::filesystem::relative(assetPath, std::filesystem::path("./Assets"));
        std::filesystem::path assetdataParentFolderPath = assetdataPath.parent_path();
        if(createPath && (!std::filesystem::exists(assetdataParentFolderPath) || !std::filesystem::is_directory(assetdataParentFolderPath))) {
            std::filesystem::create_directories(assetdataParentFolderPath);
        }

        return assetdataPath.string() + ".assetdata";
    }
}