#pragma once
#include "Asset.h"
#include "../Rendering/Texture.h"

#include <memory>

namespace glGame {
    namespace assetInternal {

        template<>
        class Asset<Texture> : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath), textureType(0, { "sRGB", "RGB" }) {
                addPublicVariable(&textureType, "TextureType");
                reload();
            }

            virtual const char* getAssetTypeString() override { return "Texture"; }
            virtual void reload() override {
                asset = std::make_unique<Texture>(filepath.c_str(), (TextureType)textureType.selection);
            }
            static AssetType GetAssetType() { return AssetType::Texture; }

            Texture* get() { return asset.get(); }
            Texture* operator->() { return asset.get(); }
        private:
            std::unique_ptr<Texture> asset;

            PublicVariableEnum textureType;
        };

    }
}