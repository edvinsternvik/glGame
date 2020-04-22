#pragma once
#include <string>

namespace glGame {

    enum class AssetType {
        Model, Texture, Script
    };

    class Asset {
    public:
        Asset(const char* name) : name(name) {}

        virtual const char* getAssetTypeString() = 0;
        virtual AssetType getAssetType() = 0;

    public:
        std::string name;
    };

}