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

    EditorController::EditorController() : m_objectPicker(Vector2i(512, 512)), m_transformGizmoSelection(TransformGizmoSelection::None) {
        
    }

    void EditorController::update(float deltatime) {
        if(Input::GetMouseKey(MOUSE_BUTTON_RIGHT)) {
            Input::SetCursorMode(CURSOR_DISABLED);

            float sensitivity = 5.0;
            Vector3 v1(0.0, Input::GetMouseDeltaX() * 0.001 * sensitivity, 0.0);
            Vector3 v2(Input::GetMouseDeltaY() * 0.001 * sensitivity, 0.0, 0.0);
            Quaternion q1 = glm::quat(v1);
            Quaternion q2 = glm::quat(v2);

            getGameObject()->transform->orientation = q2 * getGameObject()->transform->orientation * q1;
        }
        else {
            Input::SetCursorMode(CURSOR_NORMAL);
        }

        float movementSpeed = deltatime * 5.0;
        Vector3 movement(0.0, 0.0, 0.0);
        if(Input::GetKey(KEY_LEFT_SHIFT)) movementSpeed *= 2.0;
        if(Input::GetKey(KEY_W)) movement.z -= movementSpeed;
        if(Input::GetKey(KEY_S)) movement.z += movementSpeed;
        if(Input::GetKey(KEY_A)) movement.x -= movementSpeed;
        if(Input::GetKey(KEY_D)) movement.x += movementSpeed;
        if(Input::GetKey(KEY_E)) movement.y += movementSpeed;
        if(Input::GetKey(KEY_Q)) movement.y -= movementSpeed;

        glm::vec3 move = glm::inverse(getGameObject()->transform->orientation) * movement;

        getGameObject()->transform->move(move);

        if(Input::GetKeyDown(KEY_Z)) m_editor->transformType = TransformType::Move;
        if(Input::GetKeyDown(KEY_X)) m_editor->transformType = TransformType::Rotate;
        if(Input::GetKeyDown(KEY_C)) m_editor->transformType = TransformType::Scale;

        if(m_transformGizmoSelection != TransformGizmoSelection::None) {
            auto selectedObj = m_editor->getSelectedItem<GameObject>();
            if(!selectedObj.expired()) {
                GameObject* obj = selectedObj.lock().get();
                Vector3 prevPos = obj->transform->position;
                Quaternion prevOrientation = obj->transform->orientation;
                Vector3 prevScale = obj->transform->scale;
                
                Vector3 axis;
                switch(m_transformGizmoSelection) {
                    case TransformGizmoSelection::X: axis = Vector3(1.0, 0.0, 0.0); break;
                    case TransformGizmoSelection::Y: axis = Vector3(0.0, 1.0, 0.0); break;
                    case TransformGizmoSelection::Z: axis = Vector3(0.0, 0.0, -1.0); break;
                }
                Vector2 mousePosPrev = Input::GetMouseViewportPositon() - Input::GetMouseViewportDelta();
                Vector2 mousePosNow = Input::GetMouseViewportPositon();

                Vector3 localAxis = obj->transform->orientation * axis;
                Vector3 planeNormal = localAxis;
                if(m_editor->transformType != TransformType::Rotate) planeNormal = getPlaneNormal(localAxis);
                Vector3 ray1 = getScreenPosOnPlane(mousePosPrev, obj->transform->position, planeNormal);
                Vector3 ray2 = getScreenPosOnPlane(mousePosNow, obj->transform->position, planeNormal);
                Vector3 localRay1 = ray1 - obj->transform->position;
                Vector3 localRay2 = ray2 - obj->transform->position;
                Vector3 dPos = ray2 - ray1;

                switch(m_editor->transformType) {
                case TransformType::Move: {
                    obj->transform->move(localAxis * glm::dot(dPos, localAxis));
                    break;
                }
                case TransformType::Scale: {
                    Vector3 res = axis * glm::dot(dPos, axis);
                    res.z *= -1.0;
                    obj->transform->resize(res + Vector3(1.0));
                    break;
                }
                case TransformType::Rotate: {
                    float circleLength = glm::length(localRay1);
                    Vector3 ray2OnCircle = glm::normalize(localRay2) * circleLength;
                    Vector3 circleDPos = ray2OnCircle - localRay1;
                    Vector3 res = axis * glm::length(circleDPos);
                    float sign = glm::sign(glm::dot(localAxis, glm::cross(ray2OnCircle, circleDPos)));
                    obj->transform->rotate(res * sign);
                    break;
                }
                }
                if(!m_transformGizmoWasSelected) {
                    switch(m_editor->transformType) {
                    case TransformType::Move:   m_editor->actionManager.beginChangePublicVariableAction<Vector3>(&obj->transform->position, prevPos); break;
                    case TransformType::Rotate:  m_editor->actionManager.beginChangePublicVariableAction<Quaternion>(&obj->transform->orientation, prevOrientation); break;
                    case TransformType::Scale: m_editor->actionManager.beginChangePublicVariableAction<Vector3>(&obj->transform->scale, prevScale); break;
                    }
                }
            }
            m_transformGizmoWasSelected = true;
        }
        else {
            if(m_transformGizmoWasSelected) {
                auto selectedObj = m_editor->getSelectedItem<GameObject>();
                if(!selectedObj.expired()) {
                    switch(m_editor->transformType) {
                    case TransformType::Move:   m_editor->actionManager.endChangePublicVariableAction<Vector3>(&selectedObj.lock()->transform->position, selectedObj.lock()->transform->position); break;
                    case TransformType::Rotate:  m_editor->actionManager.endChangePublicVariableAction<Quaternion>(&selectedObj.lock()->transform->orientation, selectedObj.lock()->transform->orientation); break;
                    case TransformType::Scale: m_editor->actionManager.endChangePublicVariableAction<Vector3>(&selectedObj.lock()->transform->scale, selectedObj.lock()->transform->scale); break;
                    }
                }
            }
            m_transformGizmoWasSelected = false;
        }

        if(Input::GetMouseKeyDown(MOUSE_BUTTON_LEFT)) {
            if(m_editor) {
                m_objectPicker.renderColorPickerTexture(&Application::Get().renderer.previousFrameRenderData, &Application::Get().sceneManager->getActiveScene()->activeCamera.lock()->camera);
                Vector2 normalizedScreenPos = Input::GetMouseViewportPositon();
                if(normalizedScreenPos.x <= 1.0 && normalizedScreenPos.x >= 0.0 && normalizedScreenPos.y <= 1.0 && normalizedScreenPos.y >= 0.0) {
                    int objectId = m_objectPicker.getGameObjectIdFromScreen(normalizedScreenPos);

                    if(objectId < Editor::TransformGizmoMoveID) {
                        m_editor->selectItem(Application::Get().sceneManager->getActiveScene()->getGameObject(objectId));
                    }
                    else {
                        int dirId = objectId - Editor::TransformGizmoMoveID;
                        switch(dirId) {
                        case 0: m_transformGizmoSelection = TransformGizmoSelection::X; break;
                        case 1: m_transformGizmoSelection = TransformGizmoSelection::Y; break;
                        case 2: m_transformGizmoSelection = TransformGizmoSelection::Z; break;
                        }
                    }

                }
            }
            else {
                std::cout << "Error in EditorController: Editor was not set" << std::endl;
            }
        }
        else if(!Input::GetMouseKey(MOUSE_BUTTON_LEFT)) {
            m_transformGizmoSelection = TransformGizmoSelection::None;
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

    Vector3 EditorController::getPlaneNormal(Vector3 vecAlongPlane) {
        std::weak_ptr<GameObject> camera = m_editor->m_editorCameraGameObject;
        if(camera.expired()) return Vector3(0.0, 0.0, 0.0);
        Transform& cTransform = *camera.lock()->transform;
        
        Vector3 forwardDir = cTransform.orientation * Vector4(0.0, 0.0, -1.0, 0.0);
        if(glm::dot(forwardDir, vecAlongPlane) == 1.0) forwardDir = cTransform.orientation * Vector4(1.0, 0.0, 0.0, 0.0);
        Vector3 planeDirUp = glm::normalize(glm::cross(forwardDir, vecAlongPlane));
        Vector3 planeNormal = glm::cross(vecAlongPlane, planeDirUp);
        return planeNormal;
    }

    Vector3 EditorController::getScreenPosOnPlane(Vector2 normalizedScreenPos, Vector3 planeOrigin, Vector3 planeNormal) {
        std::weak_ptr<GameObject> camera = m_editor->m_editorCameraGameObject;
        if(camera.expired()) return Vector3(0.0);

        std::weak_ptr<CameraComponent> cc = camera.lock()->getComponent<CameraComponent>();
        Transform& cTransform = *camera.lock()->transform;
        glm::mat4 invProjMat = glm::inverse(cc.lock()->camera.getProjectionMatrix());
        Quaternion invOrientation = glm::inverse(cTransform.orientation);
        Vector3 lineOrigin = cTransform.position;

        Vector2 screenPos = normalizedScreenPos * Vector2(2.0, -2.0) - Vector2(1.0, -1.0);
        
        Vector3 rayDir = getRayDir(screenPos, invProjMat, invOrientation);

        Vector3 ray = castRayOnPlane(planeOrigin, planeNormal, lineOrigin, rayDir);
        if(std::isnan(ray.x) || std::isnan(ray.y) || std::isnan(ray.z)) {
            return Vector3(0.0);
        }
        return ray;
    }

}