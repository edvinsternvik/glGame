#include "Light.h"

namespace glGame {

    Light::Light(const LightType& lightType) : position(0.0, 0.0, 0.0), intensity(1.0), lightType(lightType) {

    }

    Light::Light(const LightType& lightType, const Vector3& position) : position(position), intensity(1.0), lightType(lightType) {

    }

    Light::Light(const LightType& lightType, const Vector3& position, const float& intensity) : position(position), intensity(intensity), lightType(lightType) {

    }


}