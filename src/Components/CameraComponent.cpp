#include "CameraComponent.h"
#include "../GameObject.h"
#include "Transform.h"

namespace glGame {

	CameraComponent::CameraComponent() : Component(), camera(45.0, 1920.0 / 1080.0, 0.1, 100.0) {
		addPublicVariable(&camera.fov, "Fov");
		addPublicVariable(&camera.aspectRatio, "AspectRatio");
		addPublicVariable(&camera.nearPlane, "NearPlane");
		addPublicVariable(&camera.farPlane, "FarPlane");
	}

	void CameraComponent::update(float deltatime) {
		camera.position = getGameObject()->transform->position;
		camera.rotation = getGameObject()->transform->rotation;
	}

}