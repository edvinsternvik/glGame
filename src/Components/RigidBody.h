#pragma once
#include "Component.h"

namespace redPhysics3d { class RigidBody; }

namespace glGame {

    class RigidBody : public Component {
    public:
        RigidBody();
        ~RigidBody();

        virtual std::string getName() const override { return "RigidBody"; };

        virtual void init() override;
        virtual void update(float deltatime) override;

    public:
        float gravityScale = 1.0;

    private:
        redPhysics3d::RigidBody* m_rigidBody = nullptr;
        friend class Physics3d;
    };

}