#pragma once
#include "../Math/Vector.h"

namespace glGame {

    class Light {
    public:
        Light();
        Light(const Vector3& position);
        Light(const Vector3& position, const float& intensity);
        
    public:
        Vector3 position;
        float intensity;
    };

}