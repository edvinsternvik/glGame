#include "Transform.h"

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

}