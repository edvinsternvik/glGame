#include "Editor.h"
#include "../Window.h"
#include "../Components/Camera.h"
#include "EditorController.h"
#include "../Resources/Scene.h"

namespace glGame {

    Editor::Editor(std::function<void(Event&)> eventFunction, Window* window, unsigned int editorFrameTexture, float aspectRatio) {
        m_editorGui = std::make_unique<Gui>(window->getWindow());
        m_editorGui->setEventFunction(eventFunction);
		m_viewportWindow = m_editorGui->addWindow(std::make_unique<ViewportWindow>(editorFrameTexture, aspectRatio)); // m_renderer->getEditorFrameTexture()
		m_sceneWindow = m_editorGui->addWindow(std::make_unique<SceneWindow>());
		m_propertiesWindow = m_editorGui->addWindow(std::make_unique<PropertiesWindow>());

        m_editorGameObject = std::make_unique<GameObject>("EditorGameObject");
        m_camera = m_editorGameObject->addComponent<Camera>();
        m_editorGameObject->addComponent<EditorController>();
    }

    void Editor::update(float deltatime) {
        m_editorGameObject->onUpdate(deltatime);
    }

    void Editor::render() {
        m_editorGui->OnGuiRender();
    }

    void Editor::setActiveScene(Scene* scene) {
        if(m_sceneWindow) m_sceneWindow->setScene(scene);
        if(m_propertiesWindow) m_propertiesWindow->setScene(scene);
        scene->activeCamera = m_camera;
        m_editorGameObject->onInit();
    }

}