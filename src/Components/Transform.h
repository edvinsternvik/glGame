#pragma once
#include "../Math/Vector.h"
#include "Component.h"

namespace glGame {

	class Transform : public Component {
	public:
		Transform();

		virtual std::string getName() const { return "Transform"; }

		// void move(Vector3 vector);
		void move(float x, float y, float z);

	public:
		Vector3 position, scale, rotation;
	};

}
