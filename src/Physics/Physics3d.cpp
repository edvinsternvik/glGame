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
            Vector3& position = b.first->getGameObject()->transform->position;
            Vector3& rot = b.first->getGameObject()->transform->rotation;

            b.second->position = redPhysics3d::Vector3(position.x, position.y, position.z);
            b.second->orientation = redPhysics3d::Quaternion(rot.x, rot.y, rot.z);
        }

        m_physicsWorld.stepSimulation(deltaTime);

        for(auto& b : m_rigidBodies) {
            auto rot = b.second->orientation.toEuler();
            b.first->getGameObject()->transform->position = Vector3(b.second->position.x, b.second->position.y, b.second->position.z);
            b.first->getGameObject()->transform->rotation = Vector3(rot.x, rot.y, rot.z);
        }
    }


    void Physics3d::addRigidBody(RigidBody* rigidbody) {
        auto search = m_rigidBodies.find(rigidbody);
        if(search != m_rigidBodies.end()) return;

        redPhysics3d::RigidBody* newRigidBody = m_physicsWorld.addRigidBody();
        newRigidBody->addCollisionShape<redPhysics3d::CollisionBox>();
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