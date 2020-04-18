#pragma once
#include <memory>
#include <functional>
#include "../Events/EditorEvent.h"
#include "Gui.h"

namespace glGame {

    class GameObject;
    class Scene;
    class Camera;
    class Window;

    class Editor {
    public:
        Editor(std::function<void(Event&)> eventFunction, Window* window, unsigned int editorFrameTexture, float aspectRatio);

        void update(float deltatime);
        void render();
        void setActiveScene(Scene* scene);

    private:
        std::unique_ptr<Gui> m_editorGui;
        std::unique_ptr<GameObject> m_editorGameObject;
        ViewportWindow* m_viewportWindow;
        SceneWindow* m_sceneWindow;
        PropertiesWindow* m_propertiesWindow;
        Camera* m_camera;
    };

}