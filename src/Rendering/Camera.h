#pragma once
#include "../Math/Vector.h"
#include <glm/glm.hpp>

namespace glGame {

    class Camera {
    public:
        Camera(float fov, float aspecRatio, float nearPlane, float farPlane);

        glm::mat4 getProjectionMatrix();
		glm::mat4 getViewMatrix();

    public:
        Vector3 position, rotation;
		float fov, aspectRatio, nearPlane, farPlane;
    };

}