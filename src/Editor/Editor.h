#pragma once
#include <memory>
#include <functional>
#include "../Events/EditorEvent.h"
#include "Gui.h"
#include "ActionManager.h"

namespace glGame {

    class GameObject;
    class Scene;
    class Camera;
    class Window;

    class Editor {
    public:
        Editor(std::function<void(Event&)> eventFunction, Window* window, unsigned int editorFrameTexture, float aspectRatio);

        void update(const float& deltatime, const bool& viewportFocused);
        void renderEditor();
        std::vector<GameObject*> getGizmoObjects() const;
        void setActiveScene(Scene* scene);
        void setAssetManager(AssetManager* assetManager);


    public:
        ActionManager actionManager;

    private:
        std::unique_ptr<Gui> m_editorGui;
        std::shared_ptr<GameObject> m_editorCameraGameObject;
        std::shared_ptr<GameObject> m_selectedObjectGizmoObject;

        ViewportWindow* m_viewportWindow;
        SceneWindow* m_sceneWindow;
        PropertiesWindow* m_propertiesWindow;
        AssetWindow* m_assetWindow;
        Camera* m_camera;
        Scene* m_scene;
    };

}