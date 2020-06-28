#include "ActionManager.h"

namespace glGame {

    void ActionManager::undo() {
        if(m_currentNode == &m_startNode) return;

        m_currentNode->action->unexecute();
        m_currentNode = m_currentNode->previous;
    }

    void ActionManager::redo() {
        if(!m_currentNode->next.get()) return;

        m_currentNode = m_currentNode->next.get();
        m_currentNode->action->execute();
    }

    void ActionManager::addCreateGameObjectAction(std::shared_ptr<GameObject> gameObject, Scene* scene) {
        addAction(std::make_unique<CreateGameObjectAction>(gameObject, scene));
    }

    void ActionManager::addDeleteGameObjectAction(std::shared_ptr<GameObject> gameObject, Scene* scene) {
        addAction(std::make_unique<DeleteGameObjectAction>(gameObject, scene));
    }

    void ActionManager::addCreateComponentAction(std::shared_ptr<Component> component) {
        addAction(std::make_unique<CreateComponentAction>(component));
    }

    void ActionManager::addDeleteComponentAction(std::shared_ptr<Component> component) {
        addAction(std::make_unique<DeleteComponentAction>(component));
    }


    void ActionManager::addAction(std::unique_ptr<Action> action) {
        m_currentNode->next = std::make_unique<ActionNode>();
        m_currentNode->next->action = std::move(action);
        m_currentNode->next->previous = m_currentNode;
        m_currentNode = m_currentNode->next.get();
    }



}