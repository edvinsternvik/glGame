#pragma once
#include <string>

namespace glGame {

    enum class AssetType {
        Model, Texture, Script, Shader,
        None
    };

    class Model;
    class Texture;
    class Script;
    class Shader;

    namespace assetInternal {
        class AssetT {
        public:
            AssetT(const char* filepath) : filepath(filepath) {}

            virtual const char* getAssetTypeString() = 0;

        public:
            std::string filepath;
        };

        template<class T>
        class Asset : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath), asset(filepath) { }

            virtual const char* getAssetTypeString() override { return ""; }
            static AssetType GetAssetType() { return AssetType::None; }

            T* get() { return &asset; }
            T* operator->() { return &asset; }
        private:
            T asset;
        };
    }
}