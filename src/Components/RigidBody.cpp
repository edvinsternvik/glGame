#include "RigidBody.h"
#include <RedPhysics3d/include/redPhysicsEngine3d.h>
#include "../GameObject.h"
#include "Transform.h"
#include "../Application.h"

namespace glGame {

    RigidBody::~RigidBody() {
        Application::Get().physics.removeRigidBody(this);
    }

    void RigidBody::init() {
        Application::Get().physics.addRigidBody(this);
    }

    void RigidBody::update(float deltatime) {
    }

}