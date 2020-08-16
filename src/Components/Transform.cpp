#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace glGame {

	Transform::Transform() : Component(), position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0) {
		addPublicVariable(&position, "position");
		addPublicVariable(&scale, "scale");
		addPublicVariable(&rotation, "rotation");
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
		rotation.x += vector.x;
		rotation.y += vector.y;
		rotation.z += vector.z;
	}

	void Transform::rotate(const float& x, const float& y, const float& z) {
		rotation.x += x;
		rotation.y += y;
		rotation.z += z;
	}

	glm::mat4 Transform::getTransformMatrix() const {
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z));

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));

		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale.x, scale.y, scale.z));
		
		return modelMatrix;
	}

}