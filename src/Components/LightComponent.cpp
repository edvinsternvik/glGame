#include "LightComponent.h"
#include "../Application.h"

namespace glGame {

    LightComponent::LightComponent() {
        addPublicVariable(&light.position, "Position");
        addPublicVariable(&light.intensity, "Intensity");
    }

    void LightComponent::init() {
        lightId = Application::Get().renderer.addLight(light);
    }

    void LightComponent::onRender() {
    }

    void LightComponent::onDestroy() {
        if(lightId >= 0) {
            Application::Get().renderer.deleteLight(lightId);
            lightId = -1;
        }
    }

}