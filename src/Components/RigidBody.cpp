#include "RigidBody.h"
#include <RedPhysics3d/include/redPhysicsEngine3d.h>
#include "../GameObject.h"
#include "Transform.h"
#include "../Application.h"
#include "BoxCollider.h"

namespace glGame {

    RigidBody::RigidBody() {
        addPublicVariable(&gravityScale, "gravityScale");
        addPublicVariable(&mass, "mass");
        addPublicVariable(&bounciness, "bounciness");
        addPublicVariable(&lockRotation, "lockRotation");
    }

    RigidBody::~RigidBody() {
        Application::Get().physics.removeRigidBody(this);
    }

    void RigidBody::init() {
        Application::Get().physics.addRigidBody(this);

        int numberOfComponents = getGameObject()->getComponentSize();
        for(int i = 0; i < numberOfComponents; ++i) {
            const Component* component = getGameObject()->getComponent(i).get();
            if(component->getName() == "BoxCollider") {
                ((BoxCollider*)component)->m_collisionBox = addBoxCollider();
            }
        }
    }

    void RigidBody::update(float deltatime) {
        if(!m_rigidBody) return;
        m_rigidBody->gravityScale = gravityScale;
        if(m_rigidBody->getMass() != mass) m_rigidBody->setMass(mass);
        m_rigidBody->lockRotation = lockRotation;
        m_rigidBody->bounciness = bounciness;
    }

    redPhysics3d::CollisionBox* RigidBody::addBoxCollider() {
        if(!m_rigidBody) return nullptr;
        return m_rigidBody->addCollisionShape<redPhysics3d::CollisionBox>();
    }

    void RigidBody::removeBoxCollider(BoxCollider* boxCollider) {
        if(!m_rigidBody) return;
        m_rigidBody->removeCollisionShape(boxCollider->m_collisionBox);        
    }

}