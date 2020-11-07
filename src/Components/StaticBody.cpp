#include "StaticBody.h"
#include <RedPhysics3d/include/redPhysicsEngine3d.h>
#include "../GameObject.h"
#include "Transform.h"
#include "../Application.h"
#include "BoxCollider.h"

namespace glGame {

    StaticBody::StaticBody() {
        addPublicVariable(&bounciness, "bounciness");
        addPublicVariable(&friction, "friction");
    }

    StaticBody::~StaticBody() {
        Application::Get().physics.removeStaticBody(this);
    }

    void StaticBody::init() {
        Application::Get().physics.addStaticBody(this);

        int numberOfComponents = getGameObject()->getComponentSize();
        for(int i = 0; i < numberOfComponents; ++i) {
            const Component* component = getGameObject()->getComponent(i).get();
            if(component->getName() == "BoxCollider") {
                ((BoxCollider*)component)->m_collisionBox = addBoxCollider();
            }
        }
    }

    void StaticBody::update(float deltatime) {
        if(!m_staticBody) return;
        m_staticBody->bounciness = bounciness;
        m_staticBody->friction = friction;
    }

    redPhysics3d::CollisionBox* StaticBody::addBoxCollider() {
        if(!m_staticBody) return nullptr;
        return m_staticBody->addCollisionShape<redPhysics3d::CollisionBox>();
    }

    void StaticBody::removeBoxCollider(BoxCollider* boxCollider) {
        if(!m_staticBody) return;
        m_staticBody->removeCollisionShape(boxCollider->m_collisionBox);        
    }

}