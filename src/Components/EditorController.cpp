#include "EditorController.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Input.h"
#include <math.h>
#include <glm/glm.hpp>

namespace glGame {

    EditorController::EditorController() {
        
    }

    void EditorController::update() {
        gameObject->transform->rotation.y += Input::getMouseDeltaX() * 0.1;

        float movementSpeed = 0.01;
        Vector3 movement(0.0, 0.0, 0.0);
        if(Input::getKeyDown(KEY_W)) movement.z -= movementSpeed;
        if(Input::getKeyDown(KEY_S)) movement.z += movementSpeed;
        if(Input::getKeyDown(KEY_A)) movement.x -= movementSpeed;
        if(Input::getKeyDown(KEY_D)) movement.x += movementSpeed;

        Vector3 worldMovement(0.0, 0.0, 0.0);

        float rotationY = glm::radians(-gameObject->transform->rotation.y); 
        worldMovement.z = std::cos(rotationY) * movement.z - std::sin(rotationY) * movement.x;
        worldMovement.x = std::cos(rotationY) * movement.x + std::sin(rotationY) * movement.z;

        gameObject->transform->move(worldMovement.x, worldMovement.y, worldMovement.z);
    }

}