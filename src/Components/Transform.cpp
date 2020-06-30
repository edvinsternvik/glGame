#include "Transform.h"

namespace glGame {

	Transform::Transform() : Component(), position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0) {
		addPublicVariable(&position, "position");
		addPublicVariable(&scale, "scale");
		addPublicVariable(&rotation, "rotation");
	}

	void Transform::move(Vector3 vector) {
		position.x += vector.x;
		position.y += vector.y;
		position.z += vector.z;
	}

	void Transform::move(float x, float y, float z) {
		position.x += x;
		position.y += y;
		position.z += z;
	}
}