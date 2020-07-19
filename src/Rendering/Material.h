#pragma once
#include "../Resources/ShaderAsset.h"
#include "../Resources/TextureAsset.h"

namespace glGame {

    class Material {
    public:
        Asset<Shader> shader;
        Asset<Texture> texture;
    };

}