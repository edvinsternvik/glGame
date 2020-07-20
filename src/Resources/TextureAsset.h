#pragma once
#include "Asset.h"
#include "../Rendering/Texture.h"

#include <memory>

namespace glGame {
    namespace assetInternal {

        template<>
        class Asset<Texture> : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath) {
                addPublicVariable(&textureType, "TextureType");
                asset = std::make_unique<Texture>(filepath, TextureType::sRGB);
            }

            virtual const char* getAssetTypeString() override { return "Texture"; }
            static AssetType GetAssetType() { return AssetType::Texture; }

            Texture* get() { return asset.get(); }
            Texture* operator->() { return asset.get(); }
        private:
            std::unique_ptr<Texture> asset;

            int textureType = TextureType::sRGB;
        };

    }
}