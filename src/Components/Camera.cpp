#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace glGame {

	Camera::Camera() : Component(), m_fov(90.0f), m_nearPlane(0.1f), m_farPlane(100.0f), 
		m_projectionMatrix(glm::perspective(m_fov, 1920.0f / 1080.0f, m_nearPlane, m_farPlane)),
		m_viewMatrix(glm::translate(glm::mat4(1.0), glm::vec3(0.0))) {
	}

	void Camera::update() {
		
	}

}