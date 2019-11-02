#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace glGame {

	class Camera : public Component {
	public:
		Camera();

		virtual std::string getName() const { return "Camera"; }

		virtual void update() override;

		inline const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
		inline const glm::mat4& getViewMatrix() const { return m_viewMatrix; }

	private:
		float m_fov, m_nearPlane, m_farPlane;
		glm::mat4 m_projectionMatrix, m_viewMatrix;
	};

}
