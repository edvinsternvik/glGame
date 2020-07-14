#pragma once
#include <string>
#include <memory>

namespace glGame {

    enum class AssetType {
        Model, Texture, Script, Shader,
        None
    };

    class Model;
    class Texture;
    class Script;

    namespace assetInternal {
        class AssetT {
        public:
            AssetT(const char* filepath) : filepath(filepath) {}

            virtual const char* getAssetTypeString() = 0;

        public:
            std::string filepath;
        };

        template<class T> AssetType getAssetType();
        template<> AssetType getAssetType<Model>();
        template<> AssetType getAssetType<Texture>();
        template<> AssetType getAssetType<Script>();

        template<class T> const char* getAssetTypeString();
        template<> const char* getAssetTypeString<Model>();
        template<> const char* getAssetTypeString<Texture>();
        template<> const char* getAssetTypeString<Script>();

        template<class T>
        class Asset : public AssetT {
        public:
            Asset(const char* filepath) : AssetT(filepath), asset(filepath) { }

            virtual const char* getAssetTypeString() override { return assetInternal::getAssetTypeString<T>(); }

            T* get() { return &asset; }
            T* operator->() { return &asset; }
        private:
            T asset;
        };
    }

    template<class T>
    class Asset {
    private:
        Asset(const char* filepath) : asset(std::make_shared<assetInternal::Asset<T>>(filepath)) {}
        Asset(std::weak_ptr<assetInternal::AssetT> assetPtr) : asset(std::dynamic_pointer_cast<assetInternal::Asset<T>>(assetPtr.lock())) {}
        friend class AssetManager;

    public:
        Asset() {}

        T* get() { return (asset.get() == nullptr) ? nullptr : asset->get(); }
        bool expired() const { return asset.get() == nullptr; }
        std::string getPath() const { return (expired()) ? "" : asset->filepath; }

        T* operator->() { return asset->get(); }

    private:
        std::shared_ptr<assetInternal::Asset<T>> asset; 
    };

}