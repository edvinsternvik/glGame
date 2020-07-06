#include "EditorController.h"
#include "../GameObject.h"
#include "../Components/Transform.h"
#include "../Input.h"
#include <math.h>
#include <glm/glm.hpp>

namespace glGame {

    EditorController::EditorController() {
        
    }

    void EditorController::update(float deltatime) {
        if(Input::GetMouseKey(MOUSE_BUTTON_LEFT)) {
            Input::SetCursorMode(CURSOR_DISABLED);
            getGameObject()->transform->rotation.y += Input::GetMouseDeltaX() * 0.1;
        }
        else {
            Input::SetCursorMode(CURSOR_NORMAL);
        }

        float movementSpeed = deltatime * 5.0;
        Vector3 movement(0.0, 0.0, 0.0);
        if(Input::GetKey(KEY_W)) movement.z -= movementSpeed;
        if(Input::GetKey(KEY_S)) movement.z += movementSpeed;
        if(Input::GetKey(KEY_A)) movement.x -= movementSpeed;
        if(Input::GetKey(KEY_D)) movement.x += movementSpeed;

        Vector3 worldMovement(0.0, 0.0, 0.0);

        float rotationY = glm::radians(-getGameObject()->transform->rotation.y); 
        worldMovement.z = std::cos(rotationY) * movement.z - std::sin(rotationY) * movement.x;
        worldMovement.x = std::cos(rotationY) * movement.x + std::sin(rotationY) * movement.z;

        getGameObject()->transform->move(worldMovement.x, worldMovement.y, worldMovement.z);
    }

}