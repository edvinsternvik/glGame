#pragma once
#include "AssetType.h"
#include <string>
#include <memory>

namespace glGame {

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