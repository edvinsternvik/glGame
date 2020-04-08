#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace glGame {

	class Camera : public Component {
	public:
		Camera();

		virtual std::string getName() const { return "Camera"; }

		inline const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
		glm::mat4 getViewMatrix();

	private:
		float m_fov, m_nearPlane, m_farPlane;
		glm::mat4 m_projectionMatrix;
	};

}
