#pragma once
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"

namespace glGame {

    class Camera {
    public:
        Camera(float fov, float aspecRatio, float nearPlane, float farPlane);

        glm::mat4 getProjectionMatrix();
		glm::mat4 getViewMatrix();

    public:
        Vector3 position;
        Quaternion orientation;
		float fov, aspectRatio, nearPlane, farPlane;
    };

}