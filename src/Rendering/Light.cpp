#include "Light.h"

namespace glGame {

    Light::Light() : position(0.0, 0.0, 0.0), intensity(1.0) {

    }

    Light::Light(const Vector3& position) : position(position), intensity(1.0) {

    }

    Light::Light(const Vector3& position, const float& intensity) : position(position), intensity(intensity) {

    }


}