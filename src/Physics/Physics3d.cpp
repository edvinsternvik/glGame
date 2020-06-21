#include "Physics3d.h"
#include "../Components/RigidBody.h"
#include "../GameObject.h"
#include "../Components/Transform.h"

namespace glGame {

    Physics3d::Physics3d() {
    }

    void Physics3d::init() {
        m_physicsWorld.init();
    }

    void Physics3d::stepSimulation(float deltaTime) {
        for(auto& b : m_rigidBodies) {
            b.second->addForce(redPhysics3d::Vector3(0.0, -1.0, 0.0));
        }

        m_physicsWorld.stepSimulation(deltaTime);

        for(auto& b : m_rigidBodies) {
            b.first->getGameObject()->transform->position = Vector3(b.second->position.x, b.second->position.y, b.second->position.z);
        }
    }


    void Physics3d::addRigidBody(RigidBody* rigidbody) {
        auto search = m_rigidBodies.find(rigidbody);
        if(search != m_rigidBodies.end()) return;

        redPhysics3d::RigidBody* newRigidBody = m_physicsWorld.addRigidBody();
        m_rigidBodies.insert(std::pair<RigidBody*, redPhysics3d::RigidBody*>(rigidbody, newRigidBody));

        Vector3& position = rigidbody->getGameObject()->transform->position;
        newRigidBody->position = redPhysics3d::Vector3(position.x, position.y, position.z);
    }

    void Physics3d::removeRigidBody(RigidBody* rigidbody) {
        auto search = m_rigidBodies.find(rigidbody);
        if(search == m_rigidBodies.end()) return;

        m_physicsWorld.removeRigidBody(search->second);
        m_rigidBodies.erase(rigidbody);
    }


}