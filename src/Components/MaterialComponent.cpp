#include "MaterialComponent.h"
#include "../Application.h"

namespace glGame {

    MaterialComponent::MaterialComponent() {
        addPublicVariable(&material.texture, "Texture");
    }

    void MaterialComponent::onRender() {
        Application::Get().renderer.setMaterial(&material);
    }

}