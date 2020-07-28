#pragma once
#include "../Math/Vector.h"

namespace glGame {

    enum LightType {
        Point = 0, Directional = 1
    };

    class Light {
    public:
        Light(const LightType& lightType);
        Light(const LightType& lightType, const Vector3& position);
        Light(const LightType& lightType, const Vector3& position, const float& intensity);
        
    public:
        Vector3 position;
        float intensity;
        LightType lightType;
    };

}