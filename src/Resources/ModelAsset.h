#pragma once
#include "Asset.h"
#include "../Rendering/Model.h"

namespace glGame {
    namespace assetInternal {

        template<>
        class Asset<Model> : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath), asset(filepath) { }

            virtual const char* getAssetTypeString() override { return "Model"; }
            static AssetType GetAssetType() { return AssetType::Model; }
            virtual bool hasData() { return true; }

            Model* get() { return &asset; }
            Model* operator->() { return &asset; }
        private:
            Model asset;
        };

    }
}