#pragma once
#include "Asset.h"
#include "../Script/Script.h"

namespace glGame {
    namespace assetInternal {

        template<>
        class Asset<Script> : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath), asset(filepath) { }

            virtual const char* getAssetTypeString() override { return "Script"; }
            static AssetType GetAssetType() { return AssetType::Script; }
            virtual bool hasData() { return true; }

            Script* get() { return &asset; }
            Script* operator->() { return &asset; }
        private:
            Script asset;
        };

    }
}