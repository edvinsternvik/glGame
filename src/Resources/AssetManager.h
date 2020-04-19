#pragma once
#include "Asset.h"
#include <unordered_map>
#include <memory>

namespace glGame {

    class AssetManager {
    public:
        AssetManager();
        ~AssetManager();

        Asset* getAsset(const char* name, AssetType type);
        void updateAssets();

    private:
        void insertAsset(const std::string& name, std::unique_ptr<Asset> asset);
    
    private:
        std::unordered_map<std::string, std::unique_ptr<Asset>> m_assets;

    };

}