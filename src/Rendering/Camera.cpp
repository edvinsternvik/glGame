#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace glGame {

    Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
        : fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane) {

    }

    glm::mat4 Camera::getProjectionMatrix() {
        return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
    }

    glm::mat4 Camera::getViewMatrix() {
		glm::mat4 viewMatrix = glm::toMat4(orientation);
		viewMatrix *= glm::translate(glm::mat4(1.0), glm::vec3(-position.x, -position.y, -position.z));

		return viewMatrix;
	}

}