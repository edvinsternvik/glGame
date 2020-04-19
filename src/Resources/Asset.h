#pragma once

namespace glGame {

    enum class AssetType {
        Model, Texture, Script
    };

    class Asset {
    public:
        virtual const char* getName() = 0;
        virtual AssetType getAssetType() = 0;
    };

}