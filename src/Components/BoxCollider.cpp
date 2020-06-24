#include "BoxCollider.h"
#include "../GameObject.h"
#include "RigidBody.h"

#include <iostream>

namespace glGame {

    BoxCollider::BoxCollider() : scale(1.0, 1.0, 1.0) {
        addPublicVariable(&localPosition, PublicVariableType::Vec3, "localPosition");
        addPublicVariable(&localRotation, PublicVariableType::Vec3, "localRotation");
        addPublicVariable(&scale, PublicVariableType::Vec3, "scale");
    }

    BoxCollider::~BoxCollider() {
        const GameObject* gameObject = getGameObject();
        if(m_collisionBox && gameObject) {
            RigidBody* rb = (RigidBody*)gameObject->getComponent("RigidBody");
            if(rb) {
                rb->removeBoxCollider(this);
            }
        }
    }


    void BoxCollider::init() {
        if(!m_collisionBox && getGameObject()) {
            RigidBody* rb = (RigidBody*)getGameObject()->getComponent("RigidBody");
            if(rb) {
                m_collisionBox = rb->addBoxCollider();
            }
        }
    }

    void BoxCollider::update(float deltaTime) {

    }

}