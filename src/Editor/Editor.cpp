#include "Editor.h"
#include "../Window.h"
#include "../Components/CameraComponent.h"
#include "EditorController.h"
#include "../Resources/Scene.h"
#include "../Components/LineRenderer.h"
#include "../Components/Transform.h"
#include "EditorRenderer.h"

#include "../Input.h"

namespace glGame {

    const int Editor::TransformGizmoMoveID = ((1 << 24) - 1) - 3;

    Editor::Editor(std::function<void(Event&)> eventFunction, Window* window, Vector2i viewportSize) : transformType(TransformType::Move) {
        m_editorRenderer = std::make_unique<EditorRenderer>(viewportSize, this);

        m_editorGui = std::make_unique<Gui>(window->getWindow());
        m_editorGui->setEventFunction(eventFunction);
		m_viewportWindow = m_editorGui->addWindow(std::make_unique<ViewportWindow>(viewportSize), this);
        m_viewportWindow->setTexture(m_editorRenderer->getFrameTexture());
		m_sceneWindow = m_editorGui->addWindow(std::make_unique<SceneWindow>(), this);
		m_propertiesWindow = m_editorGui->addWindow(std::make_unique<PropertiesWindow>(), this);
        m_assetWindow = m_editorGui->addWindow(std::make_unique<AssetWindow>(), this);

        m_editorCameraGameObject = GameObject::Create("EditorCameraGameObject", -10);
        m_camera = m_editorCameraGameObject->addComponent<CameraComponent>();
        auto m_editorController = m_editorCameraGameObject->addComponent<EditorController>();
        m_editorController->setEditor(this);
    }

    Editor::~Editor() {

    }

    void Editor::update(const float& deltatime, const bool& viewportFocused) {
        if(viewportFocused) {
            m_editorCameraGameObject->onUpdate(deltatime);
        }
        else {
            Input::SetCursorMode(CURSOR_NORMAL);
        }

        if(Input::GetKey(KEY_LEFT_CONTROL) && Input::GetKeyDown(KEY_Z)) actionManager.undo();
        if(Input::GetKey(KEY_LEFT_CONTROL) && Input::GetKeyDown(KEY_Y)) actionManager.redo();
    }

    void Editor::renderEditor() {
        m_editorGui->OnGuiRender();
        m_editorRenderer->renderEditorGizmos();
    }

    void Editor::setActiveScene(Scene* scene) {
        m_scene = scene;
        if(m_sceneWindow) m_sceneWindow->setScene(scene);
        if(m_propertiesWindow) m_propertiesWindow->setScene(scene);
        scene->activeCamera = m_camera;
        m_editorCameraGameObject->onInit();
    }

    void Editor::setAssetManager(AssetManager* assetManager) {
        m_assetWindow->setAssetManger(assetManager);
    }
}