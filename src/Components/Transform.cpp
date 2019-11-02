#include "Transform.h"

namespace glGame {

	Transform::Transform() : Component(), position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0) {
		addEditorVariable(&position, EditorVariableType::Vec3, "position");
		addEditorVariable(&scale, EditorVariableType::Vec3, "scale");
		addEditorVariable(&rotation, EditorVariableType::Vec3, "rotation");
	}

	void Transform::update() {
	}

}