#include "Asset.h"

namespace glGame {

    namespace assetInternal {
        template<class T> AssetType getAssetType() { return AssetType::None; }
        template<> AssetType getAssetType<Model>() { return AssetType::Model; }
        template<> AssetType getAssetType<Texture>() { return AssetType::Texture; }
        template<> AssetType getAssetType<Script>() { return AssetType::Script; }

        template<class T> const char* getAssetTypeString() { return ""; }
        template<> const char* getAssetTypeString<Model>() { return "Model"; }
        template<> const char* getAssetTypeString<Texture>() { return "Texture"; }
        template<> const char* getAssetTypeString<Script>() { return "Script"; }
    }

}