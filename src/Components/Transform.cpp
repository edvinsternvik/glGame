#include "Transform.h"

namespace glGame {

	Transform::Transform() : Component(), position(0, 0, 0), scale(0, 0, 0), rotation(0, 0, 0) {
		addEditorVariable(&position, EditorVariableType::Vec3);
		addEditorVariable(&rotation, EditorVariableType::Vec3);
		//addEditorVariable()
	}

	void Transform::update() {
		
	}

}