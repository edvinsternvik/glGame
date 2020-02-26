#include "Transform.h"

namespace glGame {

	Transform::Transform() : Component(), position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0) {
		addPublicVariable(&position, PublicVariableType::Vec3, "position");
		addPublicVariable(&scale, PublicVariableType::Vec3, "scale", 0.5);
		addPublicVariable(&rotation, PublicVariableType::Vec3, "rotation", 10.0);
	}

	void Transform::update() {
	}

}