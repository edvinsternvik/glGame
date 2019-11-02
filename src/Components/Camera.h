#pragma once
#include "Component.h"

namespace glGame {

	class Camera : public Component {
	public:
		Camera();

		virtual std::string getName() const { return "Camera"; }

		virtual void update() override;

	public:
	};

}
