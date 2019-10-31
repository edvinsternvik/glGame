#include "Transform.h"

namespace glGame {

	Transform::Transform() : Component(), x(0), y(0), z(0), scaleX(1), scaleY(1), scaleZ(1), rotationX(0), rotationY(0), rotationZ(0) {
		addEditorVariable(&x, EditorVariableType::Float);
	}

	void Transform::update() {
		
	}

}