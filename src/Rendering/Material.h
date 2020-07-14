#pragma once
#include "../Resources/Texture.h"
#include "../Resources/Asset.h"

namespace glGame {

    class Material {
    public:
        Asset<Texture> texture;
    };

}