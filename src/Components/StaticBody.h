#pragma once
#include "Component.h"

namespace redPhysics3d { class StaticBody; class CollisionBox; }

namespace glGame {
    class BoxCollider;

    class StaticBody : public Component {
    public:
        StaticBody();
        ~StaticBody();

        virtual std::string getName() const override { return "StaticBody"; };

        virtual void init() override;
        virtual void update(float deltatime) override;

        redPhysics3d::CollisionBox* addBoxCollider();
        void removeBoxCollider(BoxCollider* boxCollider);

    public:
        float bounciness = 0.2, friction = 0.5;

    private:
        redPhysics3d::StaticBody* m_staticBody = nullptr;
        friend class Physics3d;
    };

}