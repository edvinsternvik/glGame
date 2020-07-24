#include "LightComponent.h"
#include "../Application.h"

namespace glGame {

    LightComponent::LightComponent() {
        addPublicVariable(&light.position, "Position");
        addPublicVariable(&light.intensity, "Intensity");
    }

    void LightComponent::init() {

    }

    void LightComponent::onRender() {
        Application::Get().renderer.submit(&light);
    }

}