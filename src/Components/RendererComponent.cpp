#include "RendererComponent.h"
#include "../Math/Vector.h"
#include "../GameObject.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace glGame {

    void RendererComponent::updateModelMatrix() {
		Vector3& position = gameObject->transform->position;
		Vector3& scale = gameObject->transform->scale;
		Vector3& rotation = gameObject->transform->rotation;

		modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z));

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));

		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale.x, scale.y, scale.z));
    }

}