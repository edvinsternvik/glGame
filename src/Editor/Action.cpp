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

}