#include "Physics3d.h"
#include "../Components/RigidBody.h"
#include "../GameObject.h"
#include "../Components/Transform.h"

#include <glm/glm.hpp>

namespace glGame {

    Physics3d::Physics3d() {
    }

    void Physics3d::init() {
        m_physicsWorld.init();
    }

    void Physics3d::stepSimulation(float deltaTime) {
        for(auto& rb : m_rigidBodies) {
            Vector3& position = rb->getGameObject()->transform->position;
            Vector3& rot = rb->getGameObject()->transform->rotation;

            rb->m_rigidBody->position = redPhysics3d::Vector3(position.x, position.y, position.z);
            rb->m_rigidBody->orientation = redPhysics3d::Quaternion(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));
        }

        m_physicsWorld.stepSimulation(deltaTime);

        for(auto& rb : m_rigidBodies) {
            redPhysics3d::Vector3 rot = rb->m_rigidBody->orientation.toEuler();
            redPhysics3d::Vector3& pos = rb->m_rigidBody->position;
            rb->getGameObject()->transform->position = Vector3(pos.x, pos.y, pos.z);
            rb->getGameObject()->transform->rotation = Vector3(glm::degrees(rot.x), glm::degrees(rot.y), glm::degrees(rot.z));
        }
    }

    void Physics3d::addRigidBody(RigidBody* rigidbody) {
        auto search = m_rigidBodies.find(rigidbody);
        if(search != m_rigidBodies.end()) return;

        m_rigidBodies.insert(rigidbody);
        redPhysics3d::RigidBody* newRigidBody = m_physicsWorld.addRigidBody();
        newRigidBody->addCollisionShape<redPhysics3d::CollisionBox>();
        rigidbody->m_rigidBody = newRigidBody;

        Vector3& position = rigidbody->getGameObject()->transform->position;
        newRigidBody->position = redPhysics3d::Vector3(position.x, position.y, position.z);
    }

    void Physics3d::removeRigidBody(RigidBody* rigidbody) {
        auto search = m_rigidBodies.find(rigidbody);
        if(search == m_rigidBodies.end()) return;

        m_physicsWorld.removeRigidBody(rigidbody->m_rigidBody);
        rigidbody->m_rigidBody = nullptr;
        m_rigidBodies.erase(rigidbody);
    }


}