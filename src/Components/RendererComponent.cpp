#include "RendererComponent.h"
#include "../Math/Vector.h"
#include "../GameObject.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace glGame {

    void RendererComponent::updateModelMatrix() {
		modelMatrix = getGameObject()->transform->getTransformMatrix();
    }

}