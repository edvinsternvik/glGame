#include "Action.h"
#include "../Resources/Scene.h"

namespace glGame {

    CreateGameObjectAction::CreateGameObjectAction(std::shared_ptr<GameObject> gameObject, Scene* scene)
        : gameObject(gameObject), scene(scene) {

    }

    void CreateGameObjectAction::execute() {
        scene->createGameObject(gameObject);
    }

    void CreateGameObjectAction::unexecute() {
        scene->deleteGameObject(gameObject.get());
    }

    DeleteGameObjectAction::DeleteGameObjectAction(std::shared_ptr<GameObject> gameObject, Scene* scene)
        : gameObject(gameObject), scene(scene) {

    }

    void DeleteGameObjectAction::execute() {
        scene->deleteGameObject(gameObject.get());
    }

    void DeleteGameObjectAction::unexecute() {
        scene->createGameObject(gameObject);
    }

    CreateComponentAction::CreateComponentAction(std::shared_ptr<Component> component) : component(component) {
    }

    void CreateComponentAction::execute() {
        component->getGameObject()->addComponent(component);
    }

    void CreateComponentAction::unexecute() {
        component->getGameObject()->removeComponent(component.get());
    }

    DeleteComponentAction::DeleteComponentAction(std::shared_ptr<Component> component) : component(component) {

    }

    void DeleteComponentAction::execute() {
        component->getGameObject()->removeComponent(component.get());
    }

    void DeleteComponentAction::unexecute() {
        component->getGameObject()->addComponent(component);
    }

}