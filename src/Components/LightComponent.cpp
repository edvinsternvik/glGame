#include "LightComponent.h"
#include "../Application.h"
#include "Transform.h"

namespace glGame {

    LightComponent::LightComponent() : lightId(-1) {
        addPublicVariable(&light.intensity, "Intensity");
    }

    void LightComponent::init() {
        light.position = getGameObject()->transform->position;

        if(lightId < 0) {
            lightId = Application::Get().renderer.addLight(light);
        }
        else {
            Application::Get().renderer.updateLight((unsigned int)lightId, light);
        }
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