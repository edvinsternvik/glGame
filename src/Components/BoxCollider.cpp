#include "BoxCollider.h"
#include "../GameObject.h"
#include "RigidBody.h"

#include <RedPhysics3d/include/redPhysicsEngine3d.h>

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
        if(m_collisionBox) {
            m_collisionBox->localPosition = redPhysics3d::Vector3(localPosition.x, localPosition.y, localPosition.z);
            m_collisionBox->size = redPhysics3d::Vector3(scale.x, scale.y, scale.z);
            m_collisionBox->localOrientation = redPhysics3d::Quaternion(localRotation.x, localRotation.y, localRotation.z);
        }
    }

}