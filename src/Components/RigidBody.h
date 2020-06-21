#pragma once
#include "Component.h"

namespace redPhysics3d { class RigidBody; }

namespace glGame {

    class RigidBody : public Component {
    public:
        ~RigidBody();

        virtual std::string getName() const override { return "RigidBody"; };

        virtual void init() override;
        virtual void update(float deltatime) override;
    };

}