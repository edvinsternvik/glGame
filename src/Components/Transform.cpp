#include "Transform.h"

namespace glGame {

	Transform::Transform() : Component(), position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0) {
		addPublicVariable(&position, PublicVariableType::Vec3, "position");
		addPublicVariable(&scale, PublicVariableType::Vec3, "scale", 0.5);
		addPublicVariable(&rotation, PublicVariableType::Vec3, "rotation", 10.0);
	}

	// void Transform::move(Vector3 vector) {
	// 	position.x = vector.x;
	// 	position.y = vector.y;
	// 	position.z = vector.z;
	// }

	void Transform::move(float x, float y, float z) {
		position.x += x;
		position.y += y;
		position.z += z;
	}
}