#pragma once
#include "Asset.h"
#include <unordered_map>
#include <vector>
#include <memory>

namespace glGame {

    class AssetData {
    public:
        AssetData() {}
        AssetData(const AssetType& assetType, const std::string& assetTypeString) : assetType(assetType), assetTypeString(assetTypeString) {}
        std::weak_ptr<assetInternal::AssetT> asset;
        AssetType assetType;
        std::string assetTypeString;
    };

    class AssetManager {
    public:
        AssetManager();

        template<class T>
        Asset<T> getAsset(const std::string& assetName) {
            auto search = m_assets.find(assetName);
            if(search == m_assets.end() || search->second.assetType != assetInternal::Asset<T>::GetAssetType()) return Asset<T>();
            if(search->second.asset.expired()) {
                Asset<T> newAsset(assetName.c_str());
                if(initializePublicVariables(newAsset.asset)) {
                    newAsset.asset->reload(); // Assets with a .assetdata file currently get reloaded twice, should probably fix that..
                }
                search->second.asset = newAsset.asset;

                return newAsset;
            }

            return Asset<T>(search->second.asset);
        }
        void updateAssets();
        void saveAsset(std::shared_ptr<assetInternal::AssetT> asset);
        inline int assetCount() const { return m_assets.size(); }
        inline auto getAssetsBegin() const { return m_assets.begin(); };
        inline auto getAssetsEnd() const { return m_assets.end(); };

        static AssetManager& Get() { return *s_instance; }

    private:
        void makeEmptyAssetData(const std::string& assetName, const std::string& extension);
        bool initializePublicVariables(std::shared_ptr<assetInternal::AssetT> asset);

    private:
        std::unordered_map<std::string, AssetData> m_assets;

    private:
        static AssetManager* s_instance;
    };

}