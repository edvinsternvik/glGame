#pragma once
#include "Component.h"
#include "../Rendering/Material.h"

namespace glGame {

    class MaterialComponent : public Component {
    public:
        MaterialComponent();
        virtual std::string getName() const { return "Material"; }

        virtual void onRender() override;

    public:
        Material material;
    };

}