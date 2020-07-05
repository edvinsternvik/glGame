#pragma once

#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "ScriptComponent.h"
#include "RigidBody.h"
#include "BoxCollider.h"

namespace glGame {
    namespace componentList {
        static const char* s_components[7] = { "Transform", "Camera", "Script", "MeshRenderer", "LineRenderer", "RigidBody", "BoxCollider" };
    }
}