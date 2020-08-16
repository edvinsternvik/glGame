#pragma once
#include "Asset.h"
#include "../Rendering/Material.h"

namespace glGame {

    namespace assetInternal {

        template<>
        class Asset<Material> : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath) { 
                addPublicVariable(&asset.shader, "Shader");
                addPublicVariable(&asset.texture, "Texture");
                addPublicVariable(&asset.specularMap, "SpecularMap");
            }

            virtual const char* getAssetTypeString() override { return "Material"; }
            static AssetType GetAssetType() { return AssetType::Material; }
            virtual bool hasData() { return false; }

            Material* get() { return &asset; }
            Material* operator->() { return &asset; }
        private:
            Material asset;
        };

    }

}