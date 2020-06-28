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


}