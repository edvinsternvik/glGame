#pragma once
#include "Asset.h"
#include "../Rendering/Shader.h"

namespace glGame {
    namespace assetInternal {

        template<>
        class Asset<Shader> : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath), asset(filepath) { }

            virtual const char* getAssetTypeString() override { return "Shader"; }
            static AssetType GetAssetType() { return AssetType::Shader; }

            Shader* get() { return &asset; }
            Shader* operator->() { return &asset; }
        private:
            Shader asset;
        };

    }
}