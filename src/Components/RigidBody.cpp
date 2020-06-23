#include "RigidBody.h"
#include <RedPhysics3d/include/redPhysicsEngine3d.h>
#include "../GameObject.h"
#include "Transform.h"
#include "../Application.h"
#include "BoxCollider.h"

namespace glGame {

    RigidBody::RigidBody() {
        addPublicVariable(&gravityScale, PublicVariableType::Float, "gravityScale");
    }

    RigidBody::~RigidBody() {
        Application::Get().physics.removeRigidBody(this);
    }

    void RigidBody::init() {
        Application::Get().physics.addRigidBody(this);

        int numberOfComponents = gameObject->getComponentSize();
        for(int i = 0; i < numberOfComponents; ++i) {
            const Component* component = gameObject->getComponent(i);
            if(component->getName() == "BoxCollider") {
                ((BoxCollider*)component)->m_collisionBox = addBoxCollider();
            }
        }
    }

    void RigidBody::update(float deltatime) {
        if(!m_rigidBody) return;
        m_rigidBody->gravityScale = gravityScale;
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