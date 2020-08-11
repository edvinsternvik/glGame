#pragma once
#include "Component.h"
#include "../Rendering/Camera.h"

namespace glGame {

	class CameraComponent : public Component {
	public:
		CameraComponent();

		virtual void update(float deltatime) override;
		virtual std::string getName() const { return "Camera"; }

	public:
		Camera camera;
	};

}
