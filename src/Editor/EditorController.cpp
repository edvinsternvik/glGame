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

    Vector2 getNormalizedScreenPos(const Vector2 viewportSize);

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
        if(Input::GetKey(KEY_Q)) movement.y -= movementSpeed;

        Vector3 worldMovement(0.0, 0.0, 0.0);

        glm::vec3 f = getGameObject()->transform->orientation * glm::vec3(0.0, 0.0, -1.0);
        f.x *= -1.0;
        glm::vec3 r = glm::cross(f, glm::vec3(0.0, 1.0, 0.0));
        glm::vec3 move = f * movement.z + r * movement.x + glm::vec3(0.0, 1.0, 0.0) * movement.y;

        getGameObject()->transform->move(move.x, move.y, move.z);

        if(m_transformMoveSelected[0] || m_transformMoveSelected[1] || m_transformMoveSelected[2]) {
            auto selectedObj = m_editor->getSelectedItem<GameObject>();
            if(!selectedObj.expired()) {
                GameObject* obj = selectedObj.lock().get();
                Vector3 prevPos = obj->transform->position;
                
                Vector3 axis;
                if     (m_transformMoveSelected[0]) axis = Vector3(1.0, 0.0, 0.0);
                else if(m_transformMoveSelected[1]) axis = Vector3(0.0, 1.0, 0.0);
                else if(m_transformMoveSelected[2]) axis = Vector3(0.0, 0.0, 1.0);
                Vector3 dPos = getDeltaMouseOnPlane(obj->transform->position, axis);

                obj->transform->move(dPos * axis);
                if(!m_transformGizmoWasSelected) {
                    m_editor->actionManager.beginChangePublicVariableAction<Vector3>(&obj->transform->position, prevPos);
                }
            }
            m_transformGizmoWasSelected = true;
        }
        else {
            if(m_transformGizmoWasSelected) {
                auto selectedObj = m_editor->getSelectedItem<GameObject>();
                if(!selectedObj.expired()) {
                    m_editor->actionManager.endChangePublicVariableAction<Vector3>(&selectedObj.lock()->transform->position, selectedObj.lock()->transform->position);
                }
            }
            m_transformGizmoWasSelected = false;
        }

        if(Input::GetMouseKeyDown(MOUSE_BUTTON_LEFT)) {
            if(m_editor && m_viewportWindow) {
                m_objectPicker.renderColorPickerTexture(&Application::Get().renderer.previousFrameRenderData, &Application::Get().sceneManager->getActiveScene()->activeCamera.lock()->camera);
                Vector2 normalizedScreenPos = getNormalizedScreenPos(Vector2(m_viewportWindow->viewportWidth, m_viewportWindow->viewportHeight));
                if(normalizedScreenPos.x <= 1.0 && normalizedScreenPos.x >= 0.0 && normalizedScreenPos.y <= 1.0 && normalizedScreenPos.y >= 0.0) {
                    int objectId = m_objectPicker.getGameObjectIdFromScreen(normalizedScreenPos);

                    if(objectId < Editor::TransformGizmoMoveID) {
                        m_editor->selectItem(Application::Get().sceneManager->getActiveScene()->getGameObject(objectId));
                    }
                    else {
                        m_transformMoveSelected[objectId - Editor::TransformGizmoMoveID] = true;
                    }

                }
            }
            else {
                std::cout << "Error in EditorController: m_editor or m_viewportWindow was not set" << std::endl;
            }
        }
        else if(!Input::GetMouseKey(MOUSE_BUTTON_LEFT)) {
            m_transformMoveSelected[0] = m_transformMoveSelected[1] = m_transformMoveSelected[2] = false;
        }

    }

    Vector3 castRayOnPlane(Vector3 planePos, Vector3 planeNormal, Vector3 rayPos, Vector3 rayDirection) {
        float d = glm::dot((planePos - rayPos), planeNormal) / glm::dot(rayDirection, planeNormal);
        return rayPos + rayDirection * d;
    }

    Vector3 getRayDir(const Vector2& screenPos, const glm::mat4& invProjMat, const Quaternion& inverseOrientation) {
        Vector3 rayDirLocal = invProjMat * Vector4(screenPos, 1.0, 1.0);
        rayDirLocal = glm::normalize(Vector3(rayDirLocal));
        return inverseOrientation * rayDirLocal;
    }

    Vector3 EditorController::getDeltaMouseOnPlane(Vector3 planeOrigin, Vector3 dirAlongPlane) {
        std::weak_ptr<GameObject> camera = m_editor->m_editorCameraGameObject;
        if(camera.expired()) return Vector3(0.0);

        std::weak_ptr<CameraComponent> cc = camera.lock()->getComponent<CameraComponent>();
        Transform& cTransform = *camera.lock()->transform;
        glm::mat4 invProjMat = glm::inverse(cc.lock()->camera.getProjectionMatrix());
        Quaternion invOrientation = glm::inverse(cTransform.orientation);
        Vector3 lineOrigin = cTransform.position;

        Vector3 forwardDir = cTransform.orientation * Vector4(0.0, 0.0, -1.0, 0.0);
        Vector3 planeDirUp = glm::normalize(glm::cross(forwardDir, dirAlongPlane));
        Vector3 planeNormal = glm::cross(dirAlongPlane, planeDirUp);

        Vector2 viewportSize = Vector2(m_viewportWindow->viewportWidth, m_viewportWindow->viewportHeight);
        Vector2 normalizedScreenPos = getNormalizedScreenPos(viewportSize);
        Vector2 screenPos = normalizedScreenPos * Vector2(2.0, -2.0) - Vector2(1.0, -1.0);
        Vector2 deltaScreenPos = Input::GetMouseDelta() * Vector2(2.0, -2.0) / viewportSize;
        
        Vector3 rayDir1 = getRayDir(screenPos - deltaScreenPos, invProjMat, invOrientation);
        Vector3 rayDir2 = getRayDir(screenPos                 , invProjMat, invOrientation);

        Vector3 ray1 = castRayOnPlane(planeOrigin, planeNormal, lineOrigin, rayDir1);
        Vector3 ray2 = castRayOnPlane(planeOrigin, planeNormal, lineOrigin, rayDir2);
        Vector3 deltaRay = ray2 - ray1;
        if(std::isnan(deltaRay.x) || std::isnan(deltaRay.y) || std::isnan(deltaRay.z)) {
            return Vector3(0.0);
        }
        return deltaRay;
    }

    Vector2 getNormalizedScreenPos(const Vector2 viewportSize) {
        Vector2 screenPos = Input::GetMousePosition();
        return Vector2(screenPos.x / viewportSize.x, screenPos.y / viewportSize.y);
    }

}