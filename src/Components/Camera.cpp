#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../GameObject.h"
#include "Transform.h"

namespace glGame {

	Camera::Camera() : Component(), m_fov(45.0f), m_nearPlane(0.1f), m_farPlane(100.0f), 
		m_projectionMatrix(glm::perspective(m_fov, 1920.0f / 1080.0f, m_nearPlane, m_farPlane)) {
	}

	glm::mat4 Camera::getViewMatrix() {
		Vector3 position = getGameObject()->transform->position;
		Vector3 rotation = getGameObject()->transform->rotation;
		glm::mat4 viewMatrix = glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1, 0, 0));
		viewMatrix *= glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0, 1, 0));
		viewMatrix *= glm::rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0, 0, 1));
		viewMatrix *= glm::translate(glm::mat4(1.0), glm::vec3(-position.x, -position.y, -position.z));

		return viewMatrix;
	}

}