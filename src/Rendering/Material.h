#pragma once
#include "Texture.h"
#include "Shader.h"
#include "../Resources/Asset.h"

namespace glGame {

    class Material {
    public:
        Asset<Shader> shader;
        Asset<Texture> texture;
    };

}