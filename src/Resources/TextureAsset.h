#pragma once
#include "Asset.h"
#include "../Rendering/Texture.h"

namespace glGame {
    namespace assetInternal {

        template<>
        class Asset<Texture> : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath), asset(filepath) { }

            virtual const char* getAssetTypeString() override { return "Texture"; }
            static AssetType GetAssetType() { return AssetType::Texture; }

            Texture* get() { return &asset; }
            Texture* operator->() { return &asset; }
        private:
            Texture asset;
        };

    }
}