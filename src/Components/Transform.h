#pragma once
#include "../Math/Vector.h"
#include "Component.h"

namespace glGame {

	class Transform : public Component {
	public:
		Transform();

		virtual std::string getName() const { return "Transform"; }

		virtual void update() override;

	public:
		Vector3 position, scale, rotation;
	};

}
