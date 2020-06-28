#pragma once
#include <memory>

namespace glGame {

    class GameObject;
    class Scene;

    enum class ActionType {
        ChangePublicVariableActionType, CreateGameObjectActionType, DeleteGameActionObject
    };

    class Action {
    public:
        virtual ActionType getActionType() const = 0;

        virtual void execute() = 0;
        virtual void unexecute() = 0;

    private:
        bool m_completed = false;

        friend class ActionManager;
    };

    template<typename T>
    class ChangePublicVariableAction : public Action {
    public:
        ChangePublicVariableAction(T* publicVariable, T before) : publicVariable(publicVariable), before(before) {}

        virtual ActionType getActionType() const override { return ActionType::ChangePublicVariableActionType; }

        virtual void execute() override {
            *publicVariable = after;
        }

        virtual void unexecute() override {
            *publicVariable = before;
        }

    public:
        T* publicVariable;
        T before;
        T after;
    };

    class CreateGameObjectAction : public Action {
    public:
        CreateGameObjectAction(std::shared_ptr<GameObject> gameObject, Scene* scene);
        virtual ActionType getActionType() const override { return ActionType::CreateGameObjectActionType; }
        virtual void execute() override;
        virtual void unexecute() override;

    public:
        std::shared_ptr<GameObject> gameObject;
        Scene* scene;
    };

    class DeleteGameObjectAction : public Action {
    public:
        DeleteGameObjectAction(std::shared_ptr<GameObject> gameObject, Scene* scene);
        virtual ActionType getActionType() const override { return ActionType::CreateGameObjectActionType; }
        virtual void execute() override;
        virtual void unexecute() override;

    public:
        std::shared_ptr<GameObject> gameObject;
        Scene* scene;
    };

}