#include "EditorController.h"
#include "Editor.h"
#include "../GameObject.h"
#include "../Components/Transform.h"
#include "../Input.h"
#include "../Application.h"
#include "../Components/CameraComponent.h"
#include <math.h>
#include <glm/glm.hpp>

namespace glGame {

    EditorController::EditorController() : m_objectPicker(Vector2i(512, 512)) {
        
    }

    void EditorController::update(float deltatime) {
        if(Input::GetMouseKey(MOUSE_BUTTON_RIGHT)) {
            Input::SetCursorMode(CURSOR_DISABLED);
            getGameObject()->transform->rotate(0.0, Input::GetMouseDeltaX() * 0.005, 0.0);
        }
        else {
            Input::SetCursorMode(CURSOR_NORMAL);
        }

        float movementSpeed = deltatime * 5.0;
        Vector3 movement(0.0, 0.0, 0.0);
        if(Input::GetKey(KEY_W)) movement.z += movementSpeed;
        if(Input::GetKey(KEY_S)) movement.z -= movementSpeed;
        if(Input::GetKey(KEY_A)) movement.x -= movementSpeed;
        if(Input::GetKey(KEY_D)) movement.x += movementSpeed;
        if(Input::GetKey(KEY_D)) movement.x += movementSpeed;
        if(Input::GetKey(KEY_SPACE)) movement.y += movementSpeed;
        if(Input::GetKey(KEY_LEFT_CONTROL)) movement.y -= movementSpeed;

        Vector3 worldMovement(0.0, 0.0, 0.0);

        glm::vec3 f = getGameObject()->transform->orientation * glm::vec3(0.0, 0.0, -1.0);
        f.x *= -1.0;
        glm::vec3 r = glm::cross(f, glm::vec3(0.0, 1.0, 0.0));
        glm::vec3 move = f * movement.z + r * movement.x + glm::vec3(0.0, 1.0, 0.0) * movement.y;

        getGameObject()->transform->move(move.x, move.y, move.z);

        if(Input::GetMouseKeyDown(MOUSE_BUTTON_LEFT)) {
            if(m_editor && m_viewportWindow) {
                m_objectPicker.renderColorPickerTexture(&Application::Get().renderer.previousFrameRenderData, &Application::Get().sceneManager->getActiveScene()->activeCamera.lock()->camera);
                Vector2 screenPos = Input::GetMousePosition();
                Vector2 normalizedScreenPos(screenPos.x / m_viewportWindow->viewportWidth, screenPos.y / m_viewportWindow->viewportHeight);
                if(normalizedScreenPos.x <= 1.0 && normalizedScreenPos.x >= 0.0 && normalizedScreenPos.y <= 1.0 && normalizedScreenPos.y >= 0.0) {
                    int objectId = m_objectPicker.getGameObjectIdFromScreen(normalizedScreenPos);
                    m_editor->selectItem(Application::Get().sceneManager->getActiveScene()->getGameObject(objectId));
                }
            }
            else {
                std::cout << "Error in EditorController: m_editor or m_viewportWindow was not set" << std::endl;
            }
        }

    }

}