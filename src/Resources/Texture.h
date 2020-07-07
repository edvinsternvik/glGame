#pragma once
#include "Asset.h"
#include <memory>

namespace glGame {

    class Texture : public Asset {
    public:
        Texture(const char* filepath);
        ~Texture();

        virtual const char* getAssetTypeString() { return "Texture"; }
        static AssetType getAssetType() { return AssetType::Texture; }

        void bind();
        void unbind();

    private:
        unsigned int m_textureId;
        int m_imageWidth, m_imageHeight, m_channels;
    };

    using TextureAsset = std::shared_ptr<Texture>;

}