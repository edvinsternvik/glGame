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
		glm::mat4 viewMatrix = glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1, 0, 0));
		viewMatrix *= glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0, 1, 0));
		viewMatrix *= glm::rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0, 0, 1));
		viewMatrix *= glm::translate(glm::mat4(1.0), glm::vec3(-position.x, -position.y, -position.z));

		return viewMatrix;
	}

}