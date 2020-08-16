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

        if(Input::GetMouseKeyDown(MOUSE_BUTTON_LEFT)) {
            if(m_editor && m_viewportWindow) {
                m_objectPicker.renderColorPickerTexture(&Application::Get().renderer.previousFrameRenderData, &Application::Get().sceneManager->getActiveScene()->activeCamera.lock()->camera);
                Vector2 screenPos = Input::GetMousePosition();
                int objectId = m_objectPicker.getGameObjectIdFromScreen(Vector2(screenPos.x / m_viewportWindow->viewportWidth, screenPos.y / m_viewportWindow->viewportHeight));
                m_editor->selectItem(Application::Get().sceneManager->getActiveScene()->getGameObject(objectId));
            }
            else {
                std::cout << "Error in EditorController: m_editor or m_viewportWindow was not set" << std::endl;
            }
        }

    }

}