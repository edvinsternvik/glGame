#pragma once
#include "Texture.h"
#include "../Resources/Asset.h"

namespace glGame {

    class Material {
    public:
        Asset<Texture> texture;
    };

}