#pragma once
#include <unordered_map> 

#include <memory>

#include <RedPhysics3d/include/redPhysicsEngine3d.h>

namespace glGame {

    class RigidBody;
    class StaticBody;

    class Physics3d {
    public:
        Physics3d();

        void init();
        void stepSimulation(float deltaTime);

        void addRigidBody(RigidBody* rigidbody);
        void removeRigidBody(RigidBody* rigidbody);

    private:
        redPhysics3d::PhysicsWorld m_physicsWorld;
        std::unordered_map<RigidBody*, redPhysics3d::RigidBody*> m_rigidBodies;
    };

}