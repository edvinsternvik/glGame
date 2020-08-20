#pragma once
#include "Component.h"

namespace redPhysics3d { class RigidBody; class CollisionBox; }

namespace glGame {
    class BoxCollider;

    class RigidBody : public Component {
    public:
        RigidBody();
        ~RigidBody();

        virtual std::string getName() const override { return "RigidBody"; };

        virtual void init() override;
        virtual void update(float deltatime) override;

        redPhysics3d::CollisionBox* addBoxCollider();
        void removeBoxCollider(BoxCollider* boxCollider);

    public:
        float gravityScale = 1.0, mass = 1.0;
        bool lockRotation = false;

    private:
        redPhysics3d::RigidBody* m_rigidBody = nullptr;
        friend class Physics3d;
    };

}