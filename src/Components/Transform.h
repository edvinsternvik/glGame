#pragma once
#include "Component.h"

namespace glGame {

	class Transform : public Component {
	public:
		Transform();

		virtual void update() override;

	public:
		float x, y, z;
		float scaleX, scaleY, scaleZ;
		float rotationX, rotationY, rotationZ;
	};

}
