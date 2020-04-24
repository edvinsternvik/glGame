#pragma once
#include "Asset.h"
#include <unordered_map>
#include <memory>

namespace glGame {

    class AssetManager {
    public:
        AssetManager();
        ~AssetManager();

        Asset* getAsset(unsigned int id, AssetType type);
        void updateAssets();
        inline int assetCount() const { return m_assets.size(); }
        inline auto getAssetsBegin() const { return m_assets.begin(); };
        inline auto getAssetsEnd() const { return m_assets.end(); };

        static AssetManager& Get() { return *s_instance; }

    private:
        void initFromFile();
        void insertAsset(std::unique_ptr<Asset> asset);
    
    private:
        std::unordered_map<unsigned int, std::unique_ptr<Asset>> m_assets;
        unsigned int m_highestId;

    private:
        static AssetManager* s_instance;
    };

}