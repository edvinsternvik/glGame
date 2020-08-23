#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace glGame {

	Transform::Transform() : Component(), position(0, 0, 0), scale(1, 1, 1), orientation(glm::vec3(0.0)) {
		addPublicVariable(&position, "position");
		addPublicVariable(&scale, "scale");
		addPublicVariable(&orientation, "orientation", 10.0);
	}

	void Transform::move(const Vector3& vector) {
		position.x += vector.x;
		position.y += vector.y;
		position.z += vector.z;
	}

	void Transform::move(const float& x, const float& y, const float& z) {
		position.x += x;
		position.y += y;
		position.z += z;
	}

	void Transform::rotate(const Vector3& vector) {
		Quaternion q(glm::vec3(vector.x, vector.y, vector.z));
		orientation *= q;
	}

	void Transform::rotate(const float& x, const float& y, const float& z) {
		rotate(Vector3(x, y, z));
	}

	glm::mat4 Transform::getTransformMatrix() const {
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z));

		modelMatrix *= glm::toMat4(orientation);

		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale.x, scale.y, scale.z));
		
		return modelMatrix;
	}

	Vector3 Transform::getEulerAngles() const {
		glm::vec3 euler = glm::eulerAngles(orientation);
		return Vector3(euler.x, euler.y, euler.z);
	}

}