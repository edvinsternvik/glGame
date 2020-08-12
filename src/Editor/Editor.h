#pragma once
#include <memory>
#include <functional>
#include "../Events/EditorEvent.h"
#include "Gui.h"
#include "ActionManager.h"
#include <variant>
#include <memory>

namespace glGame {

    class GameObject;
    class Scene;
    class CameraComponent;
    class Window;
    namespace assetInternal { class AssetT; }

    using selectedItemVariant = std::variant<std::weak_ptr<GameObject>, std::weak_ptr<assetInternal::AssetT>>;

    class Editor {
    public:
        Editor(std::function<void(Event&)> eventFunction, Window* window, Vector2i viewportSize);

        void update(const float& deltatime, const bool& viewportFocused);
        void renderEditor();
        std::vector<GameObject*> getGizmoObjects() const;
        void setActiveScene(Scene* scene);
        void setAssetManager(AssetManager* assetManager);


        template<class T>
        void selectItem(std::weak_ptr<T> item) {
            m_selectedItem = item;
        }

        template<class T>
        std::weak_ptr<T> getSelectedItem() const {
            if(auto ret = std::get_if<std::weak_ptr<T>>(&m_selectedItem)) return *ret;
            return std::weak_ptr<T>();
        }


    public:
        ActionManager actionManager;

    private:
        std::unique_ptr<Gui> m_editorGui;
        std::shared_ptr<GameObject> m_editorCameraGameObject;
        std::shared_ptr<GameObject> m_selectedObjectGizmoObject;

        selectedItemVariant m_selectedItem;

        ViewportWindow* m_viewportWindow;
        SceneWindow* m_sceneWindow;
        PropertiesWindow* m_propertiesWindow;
        AssetWindow* m_assetWindow;
        std::weak_ptr<CameraComponent> m_camera;
        Scene* m_scene;
    };

}