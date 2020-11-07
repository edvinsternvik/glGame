#pragma once
#include "Component.h"
#include "../Math/Vector.h"

namespace redPhysics3d { class CollisionBox; }

namespace glGame {

    class BoxCollider : public Component {
    public:
        BoxCollider();
        ~BoxCollider();
    
        virtual std::string getName() const override { return "BoxCollider"; };

        virtual void init() override;
        virtual void update(float deltatime) override;

    public:
        Vector3 localPosition, localRotation, scale;

    private:
        redPhysics3d::CollisionBox* m_collisionBox = nullptr;
        friend class RigidBody;
        friend class StaticBody;
    };

}