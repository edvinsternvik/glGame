#pragma once
#include "Component.h"
#include "../Resources/Texture.h"

namespace glGame {

    class Material : public Component {
    public:
        Material();
        virtual std::string getName() const { return "MeshRenderer"; }

    public:
        TextureAsset texture;
    };

}