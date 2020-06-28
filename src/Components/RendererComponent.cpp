#include "RendererComponent.h"
#include "../Math/Vector.h"
#include "../GameObject.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace glGame {

	void RendererComponent::init() {
		for(auto* rc : getGameObject()->rendererComponents) {
			if(rc == this) return;
		}
		getGameObject()->rendererComponents.push_back(this);
	}

    void RendererComponent::updateModelMatrix() {
		Vector3& position = getGameObject()->transform->position;
		Vector3& scale = getGameObject()->transform->scale;
		Vector3& rotation = getGameObject()->transform->rotation;

		modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z));

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));

		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale.x, scale.y, scale.z));
    }

}