#pragma once
#include "Action.h"
#include <memory>

namespace glGame {

    class ActionNode {
    public:
        std::unique_ptr<Action> action;

        ActionNode* previous;
        std::shared_ptr<ActionNode> next;
    };

    class ActionManager {
    public:
        void undo();
        void redo();

        template<typename T>
        void beginChangePublicVariableAction(T* publicVariable, T before) {
            m_currentNode->next = std::make_shared<ActionNode>();
            m_currentNode->next->action = std::make_unique<ChangePublicVariableAction<T>>(publicVariable, before);
            m_currentNode->next->previous = m_currentNode;
        }

        template<typename T>
        void endChangePublicVariableAction(T* publicVariable, T after) {
            if(m_currentNode->next.get() != nullptr) {
                Action* action = m_currentNode->next->action.get();
                if(action->getActionType() == ActionType::ChangePublicVariableActionType && ((ChangePublicVariableAction<T>*)action)->publicVariable == publicVariable) {
                    ((ChangePublicVariableAction<T>*)action)->after = after;
                    m_currentNode = m_currentNode->next.get();
                }
            }
        }

    private:
        ActionNode m_startNode;
        ActionNode* m_currentNode = &m_startNode;
    };

}