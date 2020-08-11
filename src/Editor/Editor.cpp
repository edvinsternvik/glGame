#include "Editor.h"
#include "../Window.h"
#include "../Components/CameraComponent.h"
#include "EditorController.h"
#include "../Resources/Scene.h"
#include "../Components/LineRenderer.h"
#include "../Components/Transform.h"

#include "../Input.h"

namespace glGame {

    Editor::Editor(std::function<void(Event&)> eventFunction, Window* window, unsigned int editorFrameTexture, float aspectRatio) {
        m_editorGui = std::make_unique<Gui>(window->getWindow());
        m_editorGui->setEventFunction(eventFunction);
		m_viewportWindow = m_editorGui->addWindow(std::make_unique<ViewportWindow>(editorFrameTexture, aspectRatio), this); // m_renderer->getEditorFrameTexture()
		m_sceneWindow = m_editorGui->addWindow(std::make_unique<SceneWindow>(), this);
		m_propertiesWindow = m_editorGui->addWindow(std::make_unique<PropertiesWindow>(), this);
        m_assetWindow = m_editorGui->addWindow(std::make_unique<AssetWindow>(), this);

        m_editorCameraGameObject = GameObject::Create("EditorCameraGameObject");
        m_camera = m_editorCameraGameObject->addComponent<CameraComponent>();
        m_editorCameraGameObject->addComponent<EditorController>();

        m_selectedObjectGizmoObject = GameObject::Create("SelectedObjectGizmoObject");
        m_selectedObjectGizmoObject->addComponent<LineRenderer>()->lineLength = Vector3(1.0, 0.0, 0.0);
        m_selectedObjectGizmoObject->addComponent<LineRenderer>()->lineLength = Vector3(0.0, 1.0, 0.0);
        m_selectedObjectGizmoObject->addComponent<LineRenderer>()->lineLength = Vector3(0.0, 0.0, 1.0);
    }

    void Editor::update(const float& deltatime, const bool& viewportFocused) {
        if(viewportFocused) {
            m_editorCameraGameObject->onUpdate(deltatime);
            m_selectedObjectGizmoObject->onUpdate(deltatime);
        }
        else {
            Input::SetCursorMode(CURSOR_NORMAL);
        }

        if(Input::GetKey(KEY_LEFT_CONTROL) && Input::GetKeyDown(KEY_Z)) actionManager.undo();
        if(Input::GetKey(KEY_LEFT_CONTROL) && Input::GetKeyDown(KEY_Y)) actionManager.redo();
    }

    void Editor::renderEditor() {
        if(!getSelectedItem<GameObject>().expired()) {
            m_selectedObjectGizmoObject->transform->position = getSelectedItem<GameObject>().lock()->transform->position;
            m_selectedObjectGizmoObject->transform->rotation = getSelectedItem<GameObject>().lock()->transform->rotation;
            m_selectedObjectGizmoObject->onRender();
        }
        m_editorGui->OnGuiRender();
    }

    std::vector<GameObject*> Editor::getGizmoObjects() const {
        if(!getSelectedItem<GameObject>().expired()) return {m_selectedObjectGizmoObject.get()};
        else return {};
    }

    void Editor::setActiveScene(Scene* scene) {
        m_scene = scene;
        if(m_sceneWindow) m_sceneWindow->setScene(scene);
        if(m_propertiesWindow) m_propertiesWindow->setScene(scene);
        scene->activeCamera = m_camera;
        m_editorCameraGameObject->onInit();
        m_selectedObjectGizmoObject->onInit();
    }

    void Editor::setAssetManager(AssetManager* assetManager) {
        m_assetWindow->setAssetManger(assetManager);
    }
}