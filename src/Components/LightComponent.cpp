#include "LightComponent.h"
#include "../Application.h"
#include "Transform.h"

namespace glGame {

    LightComponent::LightComponent() : lightType(0, { "Point", "Directional"}), light(std::make_shared<Light>(LightType::Point)), lightId(-1) {
        addPublicVariable(&lightType, "LightType");
        addPublicVariable(&light->intensity, "Intensity");
        addPublicVariable(&light->direction, "Direction");
        addPublicVariable(&light->castShadows, "CastShadows");
        addPublicVariable(&light->shadowmapSize, "ShadowmapSize");
        addPublicVariable(&light->shadowSize, "ShadowSize");
        addPublicVariable(&light->shadowNearPlane, "ShadowNearPlane");
        addPublicVariable(&light->shadowFarPlane, "ShadowFarPlane");
    }

    void LightComponent::init() {
        light->lightType = (LightType)lightType.selection;
        light->position = getGameObject()->transform->position;

        light->updateLight();

        Application::Get().renderer.updateLight(light);
    }

    void LightComponent::onRender() {
    }

    void LightComponent::onDestroy() {
        Application::Get().renderer.deleteLight(light->getId());
    }

}